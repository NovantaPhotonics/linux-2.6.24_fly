/*
 * PPC440EP I/O descriptions
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright 2004 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <platforms/4xx/ibm440ep.h>
#include <asm/ocp.h>
#include <asm/ppc4xx_pic.h>

static struct ocp_func_emac_data ibm440ep_emac0_def = {
	.rgmii_idx	= -1,           /* No RGMII */
	.rgmii_mux	= -1,           /* No RGMII */
	.zmii_idx       = 0,            /* ZMII device index */
	.zmii_mux       = 0,            /* ZMII input of this EMAC */
	.mal_idx        = 0,            /* MAL device index */
	.mal_rx_chan    = 0,            /* MAL rx channel number */
	.mal_tx_chan    = 0,            /* MAL tx channel number */
	.wol_irq        = 61,		/* WOL interrupt number */
	.mdio_idx       = -1,           /* No shared MDIO */
	.tah_idx	= -1,           /* No TAH */
};

static struct ocp_func_emac_data ibm440ep_emac1_def = {
	.rgmii_idx	= -1,           /* No RGMII */
	.rgmii_mux	= -1,           /* No RGMII */
	.zmii_idx       = 0,            /* ZMII device index */
	.zmii_mux       = 1,            /* ZMII input of this EMAC */
	.mal_idx        = 0,            /* MAL device index */
	.mal_rx_chan    = 1,            /* MAL rx channel number */
	.mal_tx_chan    = 2,            /* MAL tx channel number */
	.wol_irq        = 63,  		/* WOL interrupt number */
	.mdio_idx       = -1,           /* No shared MDIO */
	.tah_idx	= -1,           /* No TAH */
};
OCP_SYSFS_EMAC_DATA()

static struct ocp_func_mal_data ibm440ep_mal0_def = {
	.num_tx_chans   = 4,  		/* Number of TX channels */
	.num_rx_chans   = 2,    	/* Number of RX channels */
	.txeob_irq	= 10,		/* TX End Of Buffer IRQ  */
	.rxeob_irq	= 11,		/* RX End Of Buffer IRQ  */
	.txde_irq	= 33,		/* TX Descriptor Error IRQ */
	.rxde_irq	= 34,		/* RX Descriptor Error IRQ */
	.serr_irq	= 32,		/* MAL System Error IRQ    */
	.dcr_base	= DCRN_MAL_BASE /* MAL0_CFG DCR number */
};
OCP_SYSFS_MAL_DATA()

static struct ocp_func_iic_data ibm440ep_iic0_def = {
	.fast_mode	= 0,		/* Use standad mode (100Khz) */
};

/* either IIC1 or SPI! */
#if !defined(CONFIG_SPI_AMCC440EP) && !defined(CONFIG_SPI_PPC4xx_MODULE)
static struct ocp_func_iic_data ibm440ep_iic1_def = {
	.fast_mode	= 0,		/* Use standad mode (100Khz) */
};
#endif
OCP_SYSFS_IIC_DATA()

struct ocp_def core_ocp[] = {
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_OPB,
	  .index	= 0,
	  .paddr	= 0x0EF600000ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 0,
	  .paddr	= PPC440EP_UART0_ADDR,
	  .irq		= UART0_INT,
	  .pm		= IBM_CPM_UART0,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 1,
	  .paddr	= PPC440EP_UART1_ADDR,
	  .irq		= UART1_INT,
	  .pm		= IBM_CPM_UART1,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 2,
	  .paddr	= PPC440EP_UART2_ADDR,
	  .irq		= UART2_INT,
	  .pm		= IBM_CPM_UART2,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 3,
	  .paddr	= PPC440EP_UART3_ADDR,
	  .irq		= UART3_INT,
	  .pm		= IBM_CPM_UART3,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_IIC,
	  .index	= 0,
	  .paddr	= 0x0EF600700ULL,
	  .irq		= 2,
	  .pm		= IBM_CPM_IIC0,
	  .additions	= &ibm440ep_iic0_def,
	  .show		= &ocp_show_iic_data
	},
/* either IIC1 or SPI! */
#if !defined(CONFIG_SPI_AMCC440EP) && !defined(CONFIG_SPI_PPC4xx_MODULE)
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_IIC,
	  .index	= 1,
	  .paddr	= 0x0EF600800ULL,
	  .irq		= 7,
	  .pm		= IBM_CPM_IIC1,
	  .additions	= &ibm440ep_iic1_def,
	  .show		= &ocp_show_iic_data
	},
#endif
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_GPIO,
	  .index	= 0,
	  .paddr	= 0x0EF600B00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= IBM_CPM_GPIO0,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_GPIO,
	  .index	= 1,
	  .paddr	= 0x0EF600C00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_MAL,
	  .paddr	= OCP_PADDR_NA,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ibm440ep_mal0_def,
	  .show		= &ocp_show_mal_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_EMAC,
	  .index	= 0,
	  .paddr	= 0x0EF600E00ULL,
	  .irq		= 60,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ibm440ep_emac0_def,
	  .show		= &ocp_show_emac_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_EMAC,
	  .index	= 1,
	  .paddr	= 0x0EF600F00ULL,
	  .irq		= 62,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ibm440ep_emac1_def,
	  .show		= &ocp_show_emac_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_ZMII,
	  .paddr	= 0x0EF600D00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_INVALID
	}
};

/* Polarity and triggering settings for internal interrupt sources */
struct ppc4xx_uic_settings ppc4xx_core_uic_cfg[] __initdata = {
	{ .polarity	= 0xffbffe03,
	  .triggering   = 0x00000000,
	  .ext_irq_mask = 0x000001fc,	/* IRQ0 - IRQ6 */
	},
	{ .polarity	= 0xffffc6af,
	  .triggering	= 0x06000140,
	  .ext_irq_mask = 0x00003800,	/* IRQ7 - IRQ9 */
	},
};

static struct resource usb_gadget_resources[] = {
	[0] = {
		.start	= 0x050000100ULL,
		.end 	= 0x05000017FULL,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "usb_device_irq",
		.start	= 55,
		.end	= 55,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource ohci_usb_resources[] = {
	[0] = {
		.start	= 0x0EF601000,
		.end	= 0x0EF60107F,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= 40,
		.end	= 40,
		.flags	= IORESOURCE_IRQ,
	},
};

// sbs 12-Sept-2008: Added to support SPI
static struct resource spi_resources[] = {
	[0] = {
		.start	= 0x0EF600900ULL,
		.end	= 0x0EF600906ULL,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= 8,
		.end	= 8,
		.flags	= IORESOURCE_IRQ,
	},
};
/*
static struct resource gpio_resources[] = {
	[0] = {
		.start	= 0x0EF600B00ULL,
		.end	= 0x0EF600B44ULL,
		.flags	= IORESOURCE_MEM,
	},
};
*/

static u64 dma_mask = 0xffffffffULL;

static struct platform_device ohci_usb_device = {
	.name		= "ppc-soc-ohci",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ohci_usb_resources),
	.resource	= ohci_usb_resources,
	.dev		= {
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xffffffffULL,
	}
};

static struct platform_device usb_gadget_device = {
	.name		= "musbhsfc_udc",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(usb_gadget_resources),
	.resource       = usb_gadget_resources,
	.dev		= {
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xffffffffULL,
	}
};

// sbs 12-Sept-2008: Added to support SPI
static struct platform_device amcc440ep_spi_device = {
	.name		= "amcc440ep_spi",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(spi_resources),
	.resource	= spi_resources,
	.dev		= {
				.dma_mask		= &dma_mask,
				.coherent_dma_mask	= 0xffffffffULL,
	}	
};
/*
static struct platform_device amcc440_gpio_device = {
	.name		= "amcc440ep_gpio",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(gpio_resources),
	.resource	= spi_resources,
	.dev		= {
				.dma_mask		= &dma_mask,
				.coherent_dma_mask	= 0xffffffffULL,
	}	
};
*/
static struct platform_device *ibm440ep_devs[] __initdata = {
	&ohci_usb_device,
	&usb_gadget_device,
	&amcc440ep_spi_device,		// sbs 12-Sept-2008: Added to support SPI
//	&amcc440_gpio_device,		// sbs 12-Sept-2008: Added to support SPI
};

static int __init
ibm440ep_platform_add_devices(void)
{
	return platform_add_devices(ibm440ep_devs, ARRAY_SIZE(ibm440ep_devs));
}
arch_initcall(ibm440ep_platform_add_devices);

