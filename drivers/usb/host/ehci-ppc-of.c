/*
 * EHCI HCD (Host Controller Driver) for USB.
 *
 * Bus Glue for PPC On-Chip EHCI driver on the of_platform bus
 * Tested on AMCC PPC 440EPx
 *
 * Valentine Barshak <vbarshak@ru.mvista.com>
 *
 * Based on "ehci-ppc-soc.c" by Stefan Roese <sr@denx.de>
 * and "ohci-ppc-of.c" by Sylvain Munaut <tnt@246tNt.com>
 *
 * This file is licenced under the GPL.
 */

#include <linux/signal.h>

#include <linux/of.h>
#include <linux/of_platform.h>

/* called during probe() after chip reset completes */
static int ehci_ppc_of_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	int		retval;

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;
	return ehci_reset(ehci);
}


static const struct hc_driver ehci_ppc_of_hc_driver = {
	.description = hcd_name,
	.product_desc = "OF EHCI",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset = ehci_ppc_of_setup,
	.start = ehci_run,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
#ifdef	CONFIG_PM
	.hub_suspend = ehci_hub_suspend,
	.hub_resume = ehci_hub_resume,
#endif
};


/*
 * 440EPx Errata USBH_3
 * Fix: Enable Break Memory Transfer (BMT) in INSNREG3
 */
#define PPC440EPX_EHCI0_INSREG_BMT	(0x1 << 0)
static int __devinit
ppc44x_enable_bmt(struct device_node *dn)
{
	__iomem u32 *insreg_virt;

	insreg_virt = of_iomap(dn, 1);
	if (!insreg_virt)
		return  -EINVAL;

	out_be32(insreg_virt + 3, PPC440EPX_EHCI0_INSREG_BMT);

	iounmap(insreg_virt);
	return 0;
}


static int __devinit
ehci_hcd_ppc_of_probe(struct of_device *op, const struct of_device_id *match)
{
	struct device_node *dn = op->node;
	struct usb_hcd *hcd;
	struct ehci_hcd	*ehci;
	struct resource res;
	int irq;
	int rv;

	if (usb_disabled())
		return -ENODEV;

	dev_dbg(&op->dev, "initializing PPC-OF USB Controller\n");

	rv = of_address_to_resource(dn, 0, &res);
	if (rv)
		return rv;

	hcd = usb_create_hcd(&ehci_ppc_of_hc_driver, &op->dev, "PPC-OF USB");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = res.start;
	hcd->rsrc_len = res.end - res.start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		printk(KERN_ERR __FILE__ ": request_mem_region failed\n");
		rv = -EBUSY;
		goto err_rmr;
	}

	irq = irq_of_parse_and_map(dn, 0);
	if (irq == NO_IRQ) {
		printk(KERN_ERR __FILE__ ": irq_of_parse_and_map failed\n");
		rv = -EBUSY;
		goto err_irq;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		printk(KERN_ERR __FILE__ ": ioremap failed\n");
		rv = -ENOMEM;
		goto err_ioremap;
	}

	ehci = hcd_to_ehci(hcd);

	if (of_get_property(dn, "big-endian", NULL)) {
		ehci->big_endian_mmio = 1;
		ehci->big_endian_desc = 1;
	}
	if (of_get_property(dn, "big-endian-regs", NULL))
		ehci->big_endian_mmio = 1;
	if (of_get_property(dn, "big-endian-desc", NULL))
		ehci->big_endian_desc = 1;

	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
			HC_LENGTH(ehci_readl(ehci, &ehci->caps->hc_capbase));

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	if (of_device_is_compatible(dn, "ibm,usb-ehci-440epx")) {
		rv = ppc44x_enable_bmt(dn);
		ehci_dbg(ehci, "Break Memory Transfer (BMT) is %senabled!\n",
				rv ? "NOT ": "");
	}

	rv = usb_add_hcd(hcd, irq, 0);
	if (rv == 0)
		return 0;

	iounmap(hcd->regs);
err_ioremap:
	irq_dispose_mapping(irq);
err_irq:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err_rmr:
	usb_put_hcd(hcd);

	return rv;
}


static int ehci_hcd_ppc_of_remove(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);
	dev_set_drvdata(&op->dev, NULL);

	dev_dbg(&op->dev, "stopping PPC-OF USB Controller\n");

	usb_remove_hcd(hcd);

	iounmap(hcd->regs);
	irq_dispose_mapping(hcd->irq);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);

	usb_put_hcd(hcd);

	return 0;
}


static int ehci_hcd_ppc_of_shutdown(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);

	return 0;
}


static struct of_device_id ehci_hcd_ppc_of_match[] = {
	{
		.compatible = "usb-ehci",
	},
	{},
};
MODULE_DEVICE_TABLE(of, ehci_hcd_ppc_of_match);


static struct of_platform_driver ehci_hcd_ppc_of_driver = {
	.name		= "ppc-of-ehci",
	.match_table	= ehci_hcd_ppc_of_match,
	.probe		= ehci_hcd_ppc_of_probe,
	.remove		= ehci_hcd_ppc_of_remove,
	.shutdown 	= ehci_hcd_ppc_of_shutdown,
#ifdef CONFIG_PM
	/*.suspend	= ehci_hcd_ppc_of_drv_suspend,*/
	/*.resume	= ehci_hcd_ppc_of_drv_resume,*/
#endif
	.driver		= {
		.name	= "ppc-of-ehci",
		.owner	= THIS_MODULE,
	},
};

