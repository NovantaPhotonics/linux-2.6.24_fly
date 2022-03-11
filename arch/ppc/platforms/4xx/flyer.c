/*
 * arch/ppc/platforms/4xx/flyer.c
 *
 * Synrad Flyer II board specific routines
 *
 * Copyright 2006 DENX Software Engineering, Stefan Roese <sr@denx.de>
 * 
 * Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright 2004 MontaVista Software Inc.
 *
 * John Otken <jotken@softadvances.com>
 *
 * Steve Saban <ssaban@synrad.com>
 * Copyright 2009 Synrad, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/initrd.h>
#include <linux/irq.h>
#include <linux/root_dev.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/ndfc.h>
#include <linux/mtd/physmap.h>

#include <asm/time.h>
#include <asm/todc.h>

#include <asm/machdep.h>
#include <asm/ocp.h>
#include <asm/bootinfo.h>
#include <asm/ppcboot.h>
#include <platforms/4xx/flyer.h>

#include <syslib/ibm440gx_common.h>

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
    #include <syslib/gen550.h>
#endif

/*
 * The AMCC Yosemite and Yellowstones boards share the same PC board.
 * The Yosemite has a 440EP and USB support.  The Yellowstone has a
 * 440GR and no USB hardware.  The 440EP and 440GR have the same PVR.
 */
#ifdef CONFIG_YELLOWSTONE
#define BOARDNAME  "440GR Yellowstone"
#else
#define BOARDNAME  "440EP Flyer II"
#endif

extern bd_t __res;

static struct ibm44x_clocks clocks __initdata;

/*
 * Yosemite external IRQ triggering/polarity settings
 */
unsigned char ppc4xx_uic_ext_irq_cfg[] __initdata = {
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ0: ETH0 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ1: ETH1 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ2: PCI_INTA */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ3: STTM_ALERT */
	 (IRQ_SENSE_EDGE | IRQ_POLARITY_POSITIVE), /* IRQ4: GPIO44  XILINX Interrupt*/
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ5: GND */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ6: GPIO45 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ7: GPIO46 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ8: GPIO47 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ9: GPIO48 */
};



#if defined(CONFIG_SPI_AMCC440EP) || defined(CONFIG_SPI_PPC4xx_MODULE)
#include <linux/spi/spi.h>

static struct spi_board_info flyer_spi_bi[] = {
//#if defined(CONFIG_SPI_PPC4xx_TEST) || defined(CONFIG_SPI_PPC4xx_TEST_MODULE)
    
	{
		.modalias = "amcc440ep_spidev",
		.max_speed_hz = 10 * 1000 * 1000,
		.bus_num = 0,
		.chip_select = 1,
		.mode = SPI_MODE_2,
	},
    /*
	{
		.modalias = "spi_ppc4xx_test",
		.max_speed_hz = 10 * 1000 * 1000,
		.bus_num = 0,
		.chip_select = 1,
		.mode = SPI_MODE_2,
	},
	*/
//#endif
};

static int __init flyer_spi_init(void)
{
	int numentries;
	int res;
        printk("Registering Flyer SPI Board info\n");
	numentries = sizeof(flyer_spi_bi)/sizeof(struct spi_board_info);
	res = 0;

	if (numentries)
		res = spi_register_board_info(flyer_spi_bi, numentries);
	return res;
}
device_initcall(flyer_spi_init);
#endif

static void __init
flyer_calibrate_decr(void)
{
	unsigned int freq;

	if (mfspr(SPRN_CCR1) & CCR1_TCS)
		freq = FLYER_TMRCLK;
	else
		freq = clocks.cpu;

	ibm44x_calibrate_decr(freq);
}

static int
flyer_show_cpuinfo(struct seq_file *m)
{
	seq_printf(m, "vendor\t\t: AMCC\n");
	seq_printf(m, "machine\t\t: PPC" BOARDNAME "\n");

	return 0;
}

static inline int
flyer_map_irq(struct pci_dev *dev, unsigned char idsel, unsigned char pin)
{
	static char pci_irq_table[][4] =
		/*
		 *      PCI IDSEL/INTPIN->INTLINE
		 *         A   B   C   D
		 */
		{
			{ 25, 25, 25, 25 },     /* IDSEL 1 - PCI Slot 0 */
		};

	const long min_idsel = 12, max_idsel = 12, irqs_per_slot = 4;
	return PCI_IRQ_TABLE_LOOKUP;
}

static void __init flyer_set_emacdata(void)
{
	struct ocp_def *def;
	struct ocp_func_emac_data *emacdata;

	/* Set mac_addr and phy mode for each EMAC */

	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 0);
	emacdata = def->additions;
	memcpy(emacdata->mac_addr, __res.bi_enetaddr, 6);
	emacdata->phy_mode = PHY_MODE_MII;

	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 1);
	emacdata = def->additions;
	memcpy(emacdata->mac_addr, __res.bi_enet1addr, 6);
	emacdata->phy_mode = PHY_MODE_MII;
}
/*
static int
flyer_exclude_device(unsigned char bus, unsigned char devfn)
{
	return (bus == 0 && devfn == 0);
}

#define PCI_READW(offset)				\
        (readw((void *)((u32)pci_reg_base+offset)))

#define PCI_WRITEW(value, offset)				\
	(writew(value, (void *)((u32)pci_reg_base+offset)))

#define PCI_WRITEL(value, offset)				\
	(writel(value, (void *)((u32)pci_reg_base+offset)))

// Flyer II does not have a PCI interface. Including PCI setup code casues booting problems

static void __init
flyer_setup_pci(void)
{
	void *pci_reg_base;
	unsigned long memory_size;
	memory_size = ppc_md.find_end_of_memory();

	pci_reg_base = ioremap64(FLYER_PCIL0_BASE, FLYER_PCIL0_SIZE);

	// Enable PCI I/O, Mem, and Busmaster cycles 
	PCI_WRITEW(PCI_READW(PCI_COMMAND) |
		   PCI_COMMAND_MEMORY |
		   PCI_COMMAND_MASTER, PCI_COMMAND);

	// Disable region first 
	PCI_WRITEL(0, FLYER_PCIL0_PMM0MA);

	// PLB starting addr: 0x00000000A0000000 
	PCI_WRITEL(FLYER_PCI_PHY_MEM_BASE, FLYER_PCIL0_PMM0LA);

	// PCI start addr, 0xA0000000 (PCI Address) 
	PCI_WRITEL(FLYER_PCI_MEM_BASE, FLYER_PCIL0_PMM0PCILA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM0PCIHA);

	// Enable no pre-fetch, enable region
	PCI_WRITEL(((0xffffffff -
		     (FLYER_PCI_UPPER_MEM - FLYER_PCI_MEM_BASE)) | 0x01),
		   FLYER_PCIL0_PMM0MA);

	// Disable region one 
	PCI_WRITEL(0, FLYER_PCIL0_PMM1MA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM1LA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM1PCILA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM1PCIHA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM1MA);

	// Disable region two 
	PCI_WRITEL(0, FLYER_PCIL0_PMM2MA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM2LA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM2PCILA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM2PCIHA);
	PCI_WRITEL(0, FLYER_PCIL0_PMM2MA);

	

	PCI_WRITEL(0, FLYER_PCIL0_PTM1MS);   // disabled region 1 
	PCI_WRITEL(0, FLYER_PCIL0_PTM1LA);   // begin of address map 

	memory_size = 1 << fls(memory_size - 1);

	// Size low + Enabled 
	PCI_WRITEL((0xffffffff - (memory_size - 1)) | 0x1, FLYER_PCIL0_PTM1MS);

	eieio();
	iounmap(pci_reg_base);
}
*/
/*
static void __init
flyer_setup_hose(void)
{
	unsigned int bar_response, bar;
	struct pci_controller *hose;

	flyer_setup_pci();

	hose = pcibios_alloc_controller();

	if (!hose)
		return;

	hose->first_busno = 0;
	hose->last_busno = 0xff;

	hose->pci_mem_offset = FLYER_PCI_MEM_OFFSET;

	pci_init_resource(&hose->io_resource,
			  FLYER_PCI_LOWER_IO,
			  FLYER_PCI_UPPER_IO,
			  IORESOURCE_IO,
			  "PCI host bridge");

	pci_init_resource(&hose->mem_resources[0],
			  FLYER_PCI_LOWER_MEM,
			  FLYER_PCI_UPPER_MEM,
			  IORESOURCE_MEM,
			  "PCI host bridge");

	ppc_md.pci_exclude_device = flyer_exclude_device;

	hose->io_space.start = FLYER_PCI_LOWER_IO;
	hose->io_space.end = FLYER_PCI_UPPER_IO;
	hose->mem_space.start = FLYER_PCI_LOWER_MEM;
	hose->mem_space.end = FLYER_PCI_UPPER_MEM;
	isa_io_base =
		(unsigned long)ioremap64(FLYER_PCI_IO_BASE, FLYER_PCI_IO_SIZE);
	hose->io_base_virt = (void *)isa_io_base;

	setup_indirect_pci(hose, PCIX0_CFGA, PCIX0_CFGD);
	hose->set_cfg_type = 1;

	// Zero config bars 
	for (bar = PCI_BASE_ADDRESS_1; bar <= PCI_BASE_ADDRESS_2; bar += 4) {
		early_write_config_dword(hose, hose->first_busno,
					 PCI_FUNC(hose->first_busno), bar,
					 0x00000000);
		early_read_config_dword(hose, hose->first_busno,
					PCI_FUNC(hose->first_busno), bar,
					&bar_response);
	}

	hose->last_busno = pciauto_bus_scan(hose, hose->first_busno);

	ppc_md.pci_swizzle = common_swizzle;
	ppc_md.pci_map_irq = flyer_map_irq;
}
*/
TODC_ALLOC();

static void __init
flyer_early_serial_map(void)
{
	struct uart_port port;

	/* Setup ioremapped serial port access */
	memset(&port, 0, sizeof(port));
	port.membase = ioremap64(PPC440EP_UART0_ADDR, 8);
	port.irq = 0;
	port.uartclk = clocks.uart0;
	port.regshift = 0;
	port.iotype = SERIAL_IO_MEM;
	port.flags = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
	port.line = 0;

	if (early_serial_setup(&port) != 0) {
		printk("Early serial init of port 0 failed\n");
	}

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	/* Configure debug serial access */
	gen550_init(0, &port);
#endif

	port.membase = ioremap64(PPC440EP_UART1_ADDR, 8);
	port.irq = 1;
	port.uartclk = clocks.uart1;
	port.line = 1;

	if (early_serial_setup(&port) != 0) {
		printk("Early serial init of port 1 failed\n");
	}

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	/* Configure debug serial access */
	gen550_init(1, &port);
#endif
}

static void __init
flyer_setup_arch(void)
{
	if (ppc_md.progress)
	    ppc_md.progress("setup_arch:enter", 0x700);
    
	flyer_set_emacdata();

	ibm440gx_get_clocks(&clocks, FLYER_SYSCLK, 6 * 1843200);
	ocp_sys_info.opb_bus_freq = clocks.opb;

	printk("CPU: %ul  OPB: %ul  EBC: %ul  PLB: %ul\n",clocks.cpu, clocks.opb, clocks.ebc, clocks.plb);

	/* init to some ~sane value until calibrate_delay() runs */
        loops_per_jiffy = 66666666/HZ;

	/* Setup TODC access */
	TODC_INIT(TODC_TYPE_CY14B256K,
			0,
			0,
			ioremap64(FLYER_RTC_ADDR, FLYER_RTC_SIZE),
			8);

	/* Setup PCI host bridge */
	//flyer_setup_hose();

#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start)
		ROOT_DEV = Root_RAM0;
	else
#endif
#ifdef CONFIG_ROOT_NFS
		ROOT_DEV = Root_NFS;
#else
	ROOT_DEV = Root_HDA1;
#endif

	flyer_early_serial_map();

	/* Identify the system */
	printk( "AMCC PowerPC " BOARDNAME " Platform\n" );
	//printk("flash start = %ul flashend = %ul\n",__res.bi_flashstart, __res.bi_flashsize);
}

void __init platform_init(unsigned long r3, unsigned long r4,
			  unsigned long r5, unsigned long r6, unsigned long r7)
{
	ibm44x_platform_init(r3, r4, r5, r6, r7);

	ppc_md.setup_arch = flyer_setup_arch;
	ppc_md.show_cpuinfo = flyer_show_cpuinfo;
	ppc_md.get_irq = NULL;          /* Set in ppc4xx_pic_init() */

	ppc_md.calibrate_decr = flyer_calibrate_decr;
	ppc_md.time_init = todc_time_init;
	ppc_md.set_rtc_time = todc_set_rtc_time;
	ppc_md.get_rtc_time = todc_get_rtc_time;

	ppc_md.nvram_read_val = todc_direct_read_val;
	ppc_md.nvram_write_val = todc_direct_write_val;

#ifdef CONFIG_KGDB
	ppc_md.early_serial_map = flyer_early_serial_map;
#endif
}
