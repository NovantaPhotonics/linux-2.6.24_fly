/*
 * Synrad SPI controller driver.
 *
 * Maintainer: Steve Saban
 * Modeled heavily after the AT91RM9200 driver used for Flyer
 *
 * Copyright (C) 2008 Synrad, Inc, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/workqueue.h>
#include <linux/completion.h>
#include <asm/semaphore.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/platform_device.h>
//#include <linux/fsl_devices.h>

#include <asm/byteorder.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <asm/dcr-native.h>
#include <asm/ocp.h>

#include <linux/spi/synrad_spi.h>

#undef DEBUG_SPI

static struct spi_local spi_dev[NR_SPI_DEVICES];	/* state of the SPI devices */
static int spi_enabled = 0;
static struct semaphore spi_lock;			/* protect access to SPI bus */
static int current_device = -1;				/* currently selected SPI device */
static int spi_is_ready = 1;
DECLARE_COMPLETION(transfer_complete);

static struct amcc440ep_spi_reg __iomem  *controller;
static struct amcc440ep_gpio_reg __iomem  *chipselect;

static inline void amcc440ep_write_reg32(__be32 __iomem * reg, u32 val)
{
	out_be32(reg, val);
}

static inline u32 amcc440ep_read_reg32(__be32 __iomem * reg)
{
	return in_be32(reg);
}

static inline void amcc440ep_set_reg32(__be32 __iomem * reg, u32 val)
{
        u32 after = amcc440ep_read_reg32(reg) | val;
	amcc440ep_write_reg32(reg, after);
}

static inline void amcc440ep_clear_reg32(__be32 __iomem * reg, u32 val)
{
        u32 after = amcc440ep_read_reg32(reg) & ~val;
	amcc440ep_write_reg32(reg, after);
}

static inline void amcc440ep_write_reg8(unsigned long reg, u8 val)
{
	//out_8(reg, val);
          out_8((volatile u8 *)(reg), val);
}

static inline u8 amcc440ep_read_reg8(unsigned long reg)
{
        return in_8((volatile u8 *) (reg));
	//return in_8(reg);
}

static inline void amcc440ep_set_reg8(unsigned long reg, u8 val)
{
	u8 after = amcc440ep_read_reg8(reg) | val;
	amcc440ep_write_reg8(reg, after);
}

static inline void amcc440ep_clear_reg8(unsigned long reg, u8 val)
{
	u8 after = amcc440ep_read_reg8(reg) & ~val;
	amcc440ep_write_reg8(reg, after);
}

/*
 * Access and enable the SPI bus.
 * This MUST be called before any transfers are performed.
 */

void spi_access_bus(short device)
{
   u8 regval = SPMODE_ENABLE;
   device = device-2;
   /* Ensure that requested device is valid */
   if ((device < 0) || (device >= NR_SPI_DEVICES))
       panic("synrad_spi: spi_access_bus called with invalid device");

//   if (spi_enabled == 0)         
//       amcc440ep_set_reg8((unsigned long)&controller->mode, regval); //Enable SPI
    
   spi_enabled++;
   //Lock the bus
   down(&spi_lock);
   current_device = device;
//   if(!spi_dev[device].pio_enabled)
       amcc440ep_clear_reg32(&chipselect->orr, spi_dev[device].pcs);
   
//   spi_dev[device].pio_enabled = 1;
#ifdef DEBUG_SPI
		printk("SPI CS%i enabled\n", device);
#endif
}
			
void spi_release_bus(short device)
{
    u8 regval = SPMODE_ENABLE;
    device = device-2;
    	if (device != current_device)
		panic("Synrad_spi: spi_release called with invalid device");

	/* Release the SPI bus */
	current_device = -1;
	up(&spi_lock);
	spi_enabled--;
//	if (spi_enabled == 0) 
//	    amcc440ep_clear_reg8((unsigned long)&controller->mode, regval); /* Disable SPI */
//	if(spi_dev[device].pio_enabled)
	    amcc440ep_set_reg32(&chipselect->orr, spi_dev[device].pcs);
//	spi_dev[device].pio_enabled = 0;
#ifdef DEBUG_SPI
		printk("SPI CS%i disabled\n", device);
#endif
	
}

int spi_ready(short device)
{
    device = device-2;
    if(device == current_device)
    {
	 //printk("device %d is %d\n",device, spi_is_ready)
	return spi_is_ready;
    }
//    else
	 //printk("device %d != current %d\n", device, current_device);
    return -1;
}

int spi_transfer(struct spi_transfer_list* list)
{
    char* tx;
    char* rx;
    u8 temp;
    u32 count = 0;
    spi_is_ready = 0;
    //struct spi_local *device = (struct spi_local *) &spi_dev[current_device];

    if (!list)
	panic("Synrad_spi: spi_transfer called with NULL transfer list");
    //if (current_device == -1)
	//panic("Synrad_spi: spi_transfer called without acquiring bus");
    
    /* Store transfer list */
    //device->xfers = list;
    //list->curr = 0;
    tx = list->tx[0];
    rx = list->rx[0];
    // The Synrad SPI driver does only one transfer at a timer
    #ifdef DEBUG_SPI
	printk("SPI transfer start [%i] count = %d Type = %d\n", list->nr_transfers, list->txlen[0], list->txtype);
    #endif
    spi_access_bus(2);
    if(list->txtype == SPI_ASYNC)
    {
	while(count < list->txlen[0])
	{
	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	    //printk("ATX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    count++;
	}
    }
    else
    {
	while(count < list->txlen[0])
	{
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	    
	    //printk("TX[%d]: %d\n",count,tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->TxD, tx[count]);
	    amcc440ep_write_reg8((unsigned long)&controller->control, SPCTRL_STR_ENABLE);
	    while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&controller->status)));
	    rx[count] = amcc440ep_read_reg8((unsigned long)&controller->RxD);
	    count++;
	}
    }
    spi_is_ready = 1;
    spi_release_bus(2);
    return count;
		    
}

static int __init synrad_spi_init(void)
{
    unsigned long phys_addr;
    unsigned long end_addr;
    unsigned long base_len;
    unsigned char cdm;
    int scr;
    u8 regval = DEFAULT_MODE;
    
    
    init_MUTEX(&spi_lock);
    
    // Get VM address for SPI
    phys_addr = SPI_PHYS_START;
    end_addr = SPI_PHYS_END;
    base_len = end_addr - phys_addr + 1;
	
    controller = ioremap(phys_addr, base_len);
    if (!controller)
    {
	printk(KERN_ERR "Synrad SPI ioremap FAILED\n");
	return -ENXIO;
    }
    
    // Get VM address for GPIO Chipselect
    phys_addr = GPIO_PHYS_START;
    end_addr = GPIO_PHYS_END;
    base_len = end_addr - phys_addr + 1;
	
    chipselect = ioremap(phys_addr, base_len);
    if (!chipselect)
    {
	printk(KERN_ERR "Synrad GPIO ioremap FAILED\n");
	goto out_unmap_controller;
    }
	
    memset(&spi_dev, 0, sizeof(spi_dev));
    spi_dev[0].pcs = SET_BIT32(3);
    spi_dev[1].pcs = SET_BIT32(4);
    
    /* get the clock (Hz) for the OPB. Set in sequoia_setup_arch() */
    spi_dev[0].opb_freq = ocp_sys_info.opb_bus_freq >> 2;
    spi_dev[1].opb_freq = ocp_sys_info.opb_bus_freq >> 2;
    
    // Set up GPIO3 and GPIO4 for chip select
    amcc440ep_set_reg32(&chipselect->tcr, spi_dev[0].pcs);
    amcc440ep_set_reg32(&chipselect->tcr, spi_dev[1].pcs);
    amcc440ep_set_reg32(&chipselect->orr, spi_dev[0].pcs);
    amcc440ep_set_reg32(&chipselect->orr, spi_dev[1].pcs);
    
    // Set up SPI default values
    // set the clock 
    cdm = 0;
    // opb_freq was already divided by 4 
    scr = ((spi_dev[0].opb_freq/DEFAULT_CLK_HZ) - 1);
    cdm = scr & 0xff;
    //amcc440ep_write_reg8((unsigned long)&controller->clock, cdm);
    
    // Set the mode and Enable
    amcc440ep_write_reg8((unsigned long)&controller->mode, regval);
    printk("Synrad SPI Registered 16-Sep-2010\n");
    
    return 0;
    
out_unmap_controller:
    iounmap(controller);
        //return platform_driver_register(&synrad_spi_driver);
	
    return 0;
    //return platform_driver_register(&synrad_spi_driver);
}



static void __exit synrad_spi_exit(void)
{
    u8 regval = SPMODE_ENABLE;
    amcc440ep_clear_reg8((unsigned long)&controller->mode, regval);
    iounmap(chipselect);
    iounmap(controller);
    printk("Synrad SPI Unregistered\n");
    //platform_driver_unregister(&synrad_spi_driver);
}

EXPORT_SYMBOL(spi_access_bus);
EXPORT_SYMBOL(spi_release_bus);
EXPORT_SYMBOL(spi_transfer);
EXPORT_SYMBOL(spi_ready);

module_init(synrad_spi_init);
module_exit(synrad_spi_exit);

MODULE_AUTHOR("Steve Saban");
MODULE_DESCRIPTION("Synrad SPI Driver");
MODULE_LICENSE("GPL");
