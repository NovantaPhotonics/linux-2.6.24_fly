/*
 * AMCC 440EP SPI controller driver.
 *
 * Maintainer: Steve Saban
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
#include <asm/ocp.h>

#include <asm/FlyerII.h>
#include "spi_440ep.h"

/*
 * Default for SPI Mode:
 * 	SPI MODE 0 (active low, phase leading edge, MSB, 8-bit length
 */
#define	SPMODE_INIT_VAL (SPMODE_CP_LEADING_EDGE | SPMODE_MSB | SPMODE_ENABLE | SPMODE_CI_ACTIVELOW)
#undef ENABLE_INT

#define CS_0_PIN                   8     // GPIO Bank 1
#define CS_1_PIN                   9     // GPIO Bank 1

#define CS_0               BIT32(8)     // GPIO Bank 1
#define CS_1               BIT32(9)     // GPIO Bank 1


static const char driver_name[] = "amcc440ep_spi";
static u8 count;

/* SPI Controller driver's private data. */
struct amcc440ep_spi {
        /* bitbang has to be first */
	struct spi_bitbang bitbang;
	struct completion done;

	int             irq;
	unsigned int opb_freq;
		/* for transfers */
	int len;
	int count;
	/* data buffers */
	const unsigned char *tx;
	unsigned char *rx;
	struct amcc440ep_spi_reg __iomem  *spi_base;
	pAMCC440EP_GPIO __iomem  gpio_base;
	//struct amcc440ep_spi_irq_reg __iomem  *spi_irq_base;
	struct spi_master *master;
	struct device *dev;
};

/* need this so we can set the clock in the chipselect routine */
struct amcc440ep_spi_cs {
	/* speed in hertz */
	u32 speed_hz;
	/* bits per word - must be 8! */
	u8  bpw;
};


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




/* the spi->mode bits understood by this driver: */
#define MODEBITS	(SPI_CPOL | SPI_CPHA | SPI_CS_HIGH \
			| SPI_LSB_FIRST | SPI_LOOP)

static void cs_activate(struct amcc440ep_spi *as, struct spi_device *spi)
{
    u8 active = spi->mode & SPI_CS_HIGH;
    //u32 cs;
    //u32 cs_raw = 0;
    //printk("AMCC440EP SPI cs_activate entered  %d active\n", spi->chip_select);
    //cs_raw = spi->chip_select + 2;
    //cs = SET_BIT32(spi->chip_select+2);
    //cs = SET_BIT32(cs_raw);
    if(active)
    {
	if(spi->chip_select)
	    as->gpio_base->orr = as->gpio_base->orr | CS_1;
	else
	    as->gpio_base->orr = as->gpio_base->orr | CS_0;
    }
    else
    {
	if(spi->chip_select)
	    as->gpio_base->orr = as->gpio_base->orr & ~CS_1;
	else
	    as->gpio_base->orr = as->gpio_base->orr & ~CS_0;
    }		    
}
			
static void cs_deactivate(struct amcc440ep_spi *as, struct spi_device *spi)
{
    u8 active = spi->mode & SPI_CS_HIGH;
    /*
    u32 cs;
    u32 cs_raw = 0;
    cs_raw = spi->chip_select + 2;
    //printk("AMCC440EP SPI cs_deactivate entered  %d active\n", active);
    //cs = SET_BIT32(spi->chip_select+2);
    cs = SET_BIT32(cs_raw);
    */
    if(active)
    {
	if(spi->chip_select)
	    as->gpio_base->orr = as->gpio_base->orr & ~CS_1;
	else
	    as->gpio_base->orr = as->gpio_base->orr & ~CS_0;
    }
    else
    {
	if(spi->chip_select)
	    as->gpio_base->orr = as->gpio_base->orr | CS_1;
	else
	    as->gpio_base->orr = as->gpio_base->orr | CS_0;
    }	    
}

static int amcc440ep_spi_transfer_rxtx(struct spi_device *spi, struct spi_transfer *t)
{
    struct amcc440ep_spi *as = spi_master_get_devdata(spi->master);
    u8 data;
    u8 status;
    u8 rxdata;
    u32 spi_enable = 0;
    
    //printk("AMCC440EP SPI transer_rxtx entered\n");
    as->tx = t->tx_buf;
    as->rx = t->rx_buf;
    as->len = t->len;
    as->count = 0;
    u32 count = as->count;
    //printk("len: %d\n", as->len);
	/* send the first byte */
#ifdef ENABLE_INT
    data = as->tx ? as->tx[0] : 0;
    amcc440ep_write_reg8((unsigned long)&as->spi_base->TxD, data);
    amcc440ep_write_reg8((unsigned long)&as->spi_base->control, SPCTRL_STR_ENABLE);
    wait_for_completion(&as->done);
    as->bitbang.chipselect(spi, BITBANG_CS_INACTIVE);
#else
    while(count < (as->len))
    {
	data = as->tx ? as->tx[count] : 0;
	//printk("data: %x\n",data);
	amcc440ep_write_reg8((unsigned long)&as->spi_base->TxD, data);
	amcc440ep_write_reg8((unsigned long)&as->spi_base->control, SPCTRL_STR_ENABLE);
	ndelay(400);
	if (as->rx)
	{
	    ndelay(200);
	    //while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&as->spi_base->status)));
	    rxdata = amcc440ep_read_reg8((unsigned long)&as->spi_base->RxD);
	    as->rx[count] = rxdata;
	}
	count++;
	data = as->tx ? as->tx[count] : 0;
	amcc440ep_write_reg8((unsigned long)&as->spi_base->TxD, data);
	amcc440ep_write_reg8((unsigned long)&as->spi_base->control, SPCTRL_STR_ENABLE);
	ndelay(400);
	
	if (as->rx)
	{
	    //while(!(SPSTATUS_RXREADY & amcc440ep_read_reg8((unsigned long)&as->spi_base->status)));
	    ndelay(200);
	    rxdata = amcc440ep_read_reg8((unsigned long)&as->spi_base->RxD);
	    as->rx[count] = rxdata;
	}
	count++;
    }
    //amcc440ep_set_reg32(&as->gpio_base->orr, SET_BIT32(3));
    //wait_for_completion(&as->done);
    ndelay(400);
    as->bitbang.chipselect(spi, BITBANG_CS_INACTIVE);
#endif
    return as->count;  
}

static int
amcc440ep_spi_setupxfer(struct spi_device *spi, struct spi_transfer *t)
{
	struct amcc440ep_spi *as;
	struct amcc440ep_spi_cs *cs = spi->controller_state;
	int ret;
	//printk("AMCC440EP SPI setupxf_entered\n");
	as = spi_master_get_devdata(spi->master);

	cs->bpw = t ? t->bits_per_word : spi->bits_per_word;
	cs->speed_hz  = t ? t->speed_hz : spi->max_speed_hz;

	if (cs->bpw != 8) {
		dev_err(&spi->dev, "invalid bits-per-word (%d)\n", cs->bpw);
		return -EINVAL;
	}
	
	spin_lock(&as->bitbang.lock);

	if (ret < 0) {
		dev_err(&spi->dev, "setupxfer returned %d\n", ret);
		return ret;
	}

	dev_dbg(&spi->dev, "%s: mode %d, %u bpw, %d hz\n",
		__FUNCTION__, spi->mode, spi->bits_per_word,
		spi->max_speed_hz);

	
	
	
	
	if (!as->bitbang.busy) {
		as->bitbang.chipselect(spi, BITBANG_CS_INACTIVE);
		/* need to ndelay here? */
	}
	spin_unlock(&as->bitbang.lock);

	return 0;
}

static int amcc440ep_spi_setup(struct spi_device *spi)
{
	struct amcc440ep_spi *as;
	struct amcc440ep_spi_cs *cs = spi->controller_state;
	
	//printk("AMCC440EP SPI setup_entered\n");
	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

	if (spi->mode & ~MODEBITS) {
		dev_dbg(&spi->dev, "setup: unsupported mode bits %x\n",
			spi->mode & ~MODEBITS);
		return -EINVAL;
	}

	if (cs == NULL) {
		cs = kzalloc(sizeof *cs, GFP_KERNEL);
		if (!cs)
			return -ENOMEM;
		spi->controller_state = cs;
	}
	cs->speed_hz = spi->max_speed_hz;
	cs->bpw = spi->bits_per_word;
	

	return 0;
}


static void amcc440ep_spi_chipsel(struct spi_device *spi, int value)
{
    	struct amcc440ep_spi *as;
	struct amcc440ep_spi_cs *cs = spi->controller_state;
	unsigned char mode;
	unsigned char cdm, oldcdm;
	int scr;
	
	//printk("AMCC440EP SPI chipselect entered\n");
	as = spi_master_get_devdata(spi->master);

	/* disable the controller here? */
	
	//amcc440ep_clear_reg8((unsigned long)&as->spi_base->mode, SPMODE_ENABLE);

	switch (value) {
	case BITBANG_CS_INACTIVE:
		cs_deactivate(as, spi);
		break;

	case BITBANG_CS_ACTIVE:
		mode = amcc440ep_read_reg8((unsigned long)&as->spi_base->mode);
		/* clear the CLK bits */
		mode &= ~SPI_CLK_MODE3;
		switch (spi->mode & (SPI_CPHA|SPI_CPOL)) {
			case SPI_MODE_0:
				mode |= SPI_CLK_MODE0;
				break;
			case SPI_MODE_1:
				mode |= SPI_CLK_MODE1;
				break;
			case SPI_MODE_2:
				mode |= SPI_CLK_MODE2;
				break;
			case SPI_MODE_3:
				mode |= SPI_CLK_MODE3;
				break;
		}

		if (spi->mode & SPI_LSB_FIRST)
			/* this assumes that bit 7 is the LSb! */
			mode |= SPMODE_MSB;
		else
			mode &= ~SPMODE_MSB;
		
		if (spi->mode & SPI_LOOP)
		    mode |= SPMODE_LOOP;
		else
		    mode &= ~SPMODE_LOOP;

		mode |= SPMODE_ENABLE;

		/* set the clock */
		cdm = 0;
		/* opb_freq was already divided by 4 */
		scr = (as->opb_freq/cs->speed_hz) - 1;

		if (scr <= 0)
			goto cdm_done;

		cdm = scr & 0xff;

cdm_done:
		dev_dbg(&spi->dev, "setting pre-scaler to %d (hz %d)\n", cdm,
			cs->speed_hz);
		oldcdm = amcc440ep_read_reg8((unsigned long)&as->spi_base->clock);
		if (oldcdm != cdm)
		        amcc440ep_write_reg8((unsigned long)&as->spi_base->clock, cdm);

		/* write new configration */
		amcc440ep_write_reg8((unsigned long)&as->spi_base->mode, mode);

		cs_activate(as, spi);

		break;
	}
}
    

irqreturn_t amcc440ep_spi_irq(int irq, void *dev_id)
{
	struct amcc440ep_spi *as = dev_id;
	u8 status;
	u8 data;
	unsigned int count;
	u32 spi_enable;
	//printk("AMCC440EP SPI IRQ\n");
	/*
	if (irq != as->irq) {
	    printk(KERN_WARNING
		       "spi_ppc4xx_int : "
		       "Received wrong int %d. Waiting for %d\n", irq, as->irq);
		return IRQ_NONE;
	}

	

	
	if (status & SPSTATUS_BUSY) {
		dev_dbg(as->dev, "got interrupt but spi still busy?\n");
		complete(&as->done);
		return IRQ_HANDLED;
	}
	*/
	status = amcc440ep_read_reg8((unsigned long)&as->spi_base->status);
	count = as->count;
	as->count++;

	if (status & SPSTATUS_RXREADY) {
		/* Data Ready */
		data = amcc440ep_read_reg8((unsigned long)&as->spi_base->RxD);
		if (as->rx)
			as->rx[count] = data;
	}
	count++;

	if (count < as->len) {
		data = as->tx ? as->tx[count] : 0;
		amcc440ep_write_reg8((unsigned long)&as->spi_base->TxD, data);
		amcc440ep_write_reg8((unsigned long)&as->spi_base->control, SPCTRL_STR_ENABLE);
	} else
	{
	        as->gpio_base->orr = as->gpio_base->orr | CS_0;	
		complete(&as->done);
	}

	return IRQ_HANDLED;
}

static void
amcc440ep_spi_cleanup(struct spi_device *spi)
{
	kfree(spi->controller_state);
}

static void
amcc440ep_spi_enable(struct amcc440ep_spi *as)
{
    /*
	unsigned long pfc1;
	uint osrh, tsrh;

	mtdcr(SDR0_CFGADDR, SDR0_PFC1);
	pfc1 = mfdcr(SDR0_CFGDATA);
    //need to clear bit 14 to enable SPC 
	pfc1 &= ~(1 << 17);
	mtdcr(SDR0_CFGADDR, SDR0_PFC1);
	mtdcr(SDR0_CFGDATA, pfc1);

	// enable SPCDO 
	osrh = in_be32((void *)((u32)hw->gpio0 + GPIOx_OSRH));
	tsrh = in_be32((void *)((u32)hw->gpio0 + GPIOx_TSRH));
	// need to set bits 14:15 in OSRH to 0x01 
	osrh &= ~(0x03 << 16);
	osrh |= (0x01 << 16);
	out_be32((void *)((u32)hw->gpio0 + GPIOx_OSRH), osrh);

	// need to set bits 14:15 in TSRH to 0x01 
	tsrh &= ~(0x03 << 16);
	tsrh |= (0x01 << 16);
	out_be32((void *)((u32)hw->gpio0 + GPIOx_TSRH), tsrh);
	*/
}

void test(struct amcc440ep_spi *as, unsigned long cs)
{
    u8 regval;
    amcc440ep_set_reg32(&as->gpio_base->orr, cs);
    printk(KERN_INFO "AMCC440EP SPI Running Test\n");
    /* SPI controller initializations */
	amcc440ep_write_reg8((unsigned long)&as->spi_base->clock, 0);
	// Enable SPI interface 
	regval = SPMODE_INIT_VAL;	
	amcc440ep_write_reg8((unsigned long)&as->spi_base->mode, regval);
	regval = SPMODE_ENABLE;
	amcc440ep_set_reg8((unsigned long)&as->spi_base->mode, regval);
		
	
	count  = 0;	
	//amcc440ep_clear_reg32(&as->gpio_base->orr, cs);
	    
	amcc440ep_write_reg8((unsigned long)&as->spi_base->TxD, count);
	amcc440ep_write_reg8((unsigned long)&as->spi_base->control, SPCTRL_STR_ENABLE);

    
}

static int amcc440ep_spi_probe(struct platform_device *dev)
{
	struct spi_master *master;
	struct amcc440ep_spi *as;
	struct spi_bitbang *bbp;
	struct resource *regs;
	unsigned long phys_addr;
	unsigned long end_addr;
	unsigned long base_len;
	u32 spi_enable;
	unsigned int irq;
	int i;
	int ret = -ENOMEM;
	
	
	
	regs = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;
	
	irq = platform_get_irq(dev, 0);
	if (irq < 0)
		return irq;
	
	/* Get resources(memory, IRQ) associated with the device */
	
	master = spi_alloc_master(&dev->dev, sizeof *as);
	if (!master)
		goto out_free;
	
	platform_set_drvdata(dev, master);

	as = spi_master_get_devdata(master);
	as->master = spi_master_get(master);
	as->dev = &dev->dev;
	as->irq = irq;
	
	/* setup the state for the bitbang driver */
	bbp = &as->bitbang;
	bbp->master = as->master;
	bbp->setup_transfer = amcc440ep_spi_setupxfer;
	bbp->chipselect = amcc440ep_spi_chipsel;
	bbp->txrx_bufs = amcc440ep_spi_transfer_rxtx;
	bbp->use_dma = 0;
	bbp->master->setup = amcc440ep_spi_setup;
	bbp->master->cleanup = amcc440ep_spi_cleanup;
	/* only one SPI controller */
	bbp->master->bus_num = 0;
	if (bbp->master->num_chipselect == 0)
		/* GPIO has 64 signals */
		bbp->master->num_chipselect = 2;

	dev_dbg(dev, "bitbang at %p\n", bbp);

	/* get the clock (Hz) for the OPB. Set in sequoia_setup_arch() */
	as->opb_freq = ocp_sys_info.opb_bus_freq >> 2;
	/*
	as->buffer = dma_alloc_coherent(&dev->dev, BUFFER_SIZE,
					&as->buffer_dma, GFP_KERNEL);
	if (!as->buffer)
		goto out_free;
	*/
	phys_addr = regs->start;
	base_len = regs->end - regs->start + 1;
		
	as->spi_base = ioremap(phys_addr, base_len);
	if (!as->spi_base)
		goto out_free_buffer;
	
	phys_addr = GPIO_1_PHYS_START;
	end_addr = GPIO_1_PHYS_END;
	base_len = end_addr - phys_addr + 1;
	
	as->gpio_base = ioremap(phys_addr, base_len);
	if (!as->gpio_base)
	{
	        printk(KERN_ERR "AMCC440EP GPIO ioremap FAILED\n");
		goto out_unmap_spi_base;
	}
	
	
	init_completion(&as->done);
	/* Register for SPI Interrupt */
	//printk(KERN_INFO "AMCC440EP SPI request_irq\n");

	#ifdef ENABLE_INT
	ret = request_irq(as->irq, amcc440ep_spi_irq, 0,
			"amcc440ep_spi", as);	
	if (ret)
		goto out_unmap_gpio_base;
	#endif
	
	
	amcc440ep_spi_enable(as);
	ret = spi_bitbang_start(bbp);
	if (ret)
	#ifdef ENABLE_INT
	        goto out_reset_hw;
	#else
		goto out_unmap_gpio_base;
	#endif
	
	// Set up GPIO40 and GPIO41 for chip select active low
	
	as->gpio_base->osrh = as->gpio_base->osrh & ~TWOBIT32(CS_0_PIN);
	as->gpio_base->tsrh = as->gpio_base->tsrh & ~TWOBIT32(CS_0_PIN);
	as->gpio_base->odr = as->gpio_base->odr & ~CS_0;
	as->gpio_base->tcr = as->gpio_base->tcr | CS_0;
	
	as->gpio_base->osrh = as->gpio_base->osrh & ~TWOBIT32(CS_1_PIN);
	as->gpio_base->tsrh = as->gpio_base->tsrh & ~TWOBIT32(CS_1_PIN);
	as->gpio_base->odr = as->gpio_base->odr & ~CS_1;
	as->gpio_base->tcr = as->gpio_base->tcr | CS_1;
	
	// Set them high
	as->gpio_base->orr = as->gpio_base->orr | CS_0;
	as->gpio_base->orr = as->gpio_base->orr | CS_1;
	
		
	printk(KERN_INFO
	       "%s: AMCC440EP SPI Controller driver at %x (irq = %d)\n",
	       dev->dev.bus_id, (unsigned int)as->spi_base, irq);
	
//	test(as,SET_BIT32(3));
	
	return 0;
	
out_reset_hw:
	free_irq(irq, as);
out_unmap_gpio_base:
	iounmap(as->gpio_base);
out_unmap_spi_base:
	iounmap(as->spi_base);
out_free_buffer:
	//dma_free_coherent(pdev->dev, BUFFER_SIZE, as->buffer,
			//as->buffer_dma);
out_free:
	spi_master_put(master);
	
	return ret;

}

static int amcc440ep_spi_remove(struct platform_device *dev)
{
	struct amcc440ep_spi *as;
	struct spi_master *master;

	master = platform_get_drvdata(dev);
	as = spi_master_get_devdata(master);
	spi_bitbang_stop(&as->bitbang);
	//dma_free_coherent(&dev->dev, BUFFER_SIZE, as->buffer,
			//as->buffer_dma);
	free_irq(as->irq, as);
	iounmap(as->gpio_base);
	iounmap(as->spi_base);

	return 0;
}

	/* for platform bus hotplug */
static struct platform_driver synrad_spi_driver = {
        .probe = amcc440ep_spi_probe,
	.remove = amcc440ep_spi_remove,
	.driver = {
		   .name = "amcc440ep_spi",
		   .owner = THIS_MODULE,
	},
};

static int __init amcc440ep_spi_init(void)
{
        return platform_driver_register(&synrad_spi_driver);
	
	
}

static void __exit amcc440ep_spi_exit(void)
{
	platform_driver_unregister(&synrad_spi_driver);
}

module_init(amcc440ep_spi_init);
module_exit(amcc440ep_spi_exit);

MODULE_AUTHOR("Steve Saban");
MODULE_DESCRIPTION("Simple AMCC440EP SPI Driver");
MODULE_LICENSE("GPL");
