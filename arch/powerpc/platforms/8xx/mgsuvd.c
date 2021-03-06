/* arch/powerpc/platforms/8xx/mgsuvd.c
 *
 * Platform setup for the Keymile mgsuvd board
 *
 * Heiko Schocher <hs@denx.de>
 *
 * Copyright 2007 DENX Software Engineering GmbH
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/root_dev.h>

#include <linux/fs_enet_pd.h>
#include <linux/fs_uart_pd.h>
#include <linux/mii.h>

#include <asm/delay.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/page.h>
#include <asm/processor.h>
#include <asm/system.h>
#include <asm/time.h>
#include <asm/mpc8xx.h>
#include <asm/8xx_immap.h>
#include <asm/commproc.h>
#include <asm/fs_pd.h>
#include <asm/prom.h>

#include <linux/fsl_devices.h>
#include <sysdev/fsl_soc.h>
#include <linux/mod_devicetable.h>
#include <asm/of_platform.h>

extern void cpm_reset(void);
extern void mpc8xx_restart(char *cmd);
extern void mpc8xx_calibrate_decr(void);
extern int mpc8xx_set_rtc_time(struct rtc_time *tm);
extern void mpc8xx_get_rtc_time(struct rtc_time *tm);
extern void m8xx_pic_init(void);
extern unsigned int mpc8xx_get_irq(void);

static void init_smc1_uart_ioports(struct fs_uart_platform_info* fpi);
static void init_smc2_uart_ioports(struct fs_uart_platform_info* fpi);
static void init_scc3_ioports(struct fs_platform_info* ptr);

void __init mgsuvd_board_setup(void)
{
	cpm8xx_t *cp;
	u8 tmpval8;
	cp = (cpm8xx_t *)immr_map(im_cpm);

#ifdef CONFIG_SERIAL_CPM_SMC1
	clrbits32(&cp->cp_simode, 0xe0000000 >> 17);	/* brg1 */
	tmpval8 = in_8(&(cp->cp_smc[0].smc_smcm)) | (SMCM_RX | SMCM_TX);
	out_8(&(cp->cp_smc[0].smc_smcm), tmpval8);
	clrbits16(&cp->cp_smc[0].smc_smcmr, SMCMR_REN | SMCMR_TEN);
#else
	out_be16(&cp->cp_smc[0].smc_smcmr, 0);
	out_8(&cp->cp_smc[0].smc_smce, 0);
#endif

#ifdef CONFIG_SERIAL_CPM_SMC2
	clrbits32(&cp->cp_simode, 0xe0000000 >> 1);
	setbits32(&cp->cp_simode, 0x20000000 >> 1);	/* brg2 */
	tmpval8 = in_8(&(cp->cp_smc[1].smc_smcm)) | (SMCM_RX | SMCM_TX);
	out_8(&(cp->cp_smc[1].smc_smcm), tmpval8);
	clrbits16(&cp->cp_smc[1].smc_smcmr, SMCMR_REN | SMCMR_TEN);

	init_smc2_uart_ioports(0);
#else
	out_be16(&cp->cp_smc[1].smc_smcmr, 0);
	out_8(&cp->cp_smc[1].smc_smce, 0);
#endif
	immr_unmap(cp);
}

void init_fec_ioports(struct fs_platform_info *fpi)
{
	return;
}

static void init_scc3_ioports(struct fs_platform_info* fpi)
{
	iop8xx_t *io_port;
	cpm8xx_t *cp;

	io_port = (iop8xx_t *)immr_map(im_ioport);
	cp = (cpm8xx_t *)immr_map(im_cpm);

	/* Configure port A pins for Txd and Rxd.
	 */
	setbits16(&io_port->iop_papar, PA_ENET_RXD | PA_ENET_TXD);
	clrbits16(&io_port->iop_padir, PA_ENET_RXD | PA_ENET_TXD);
	clrbits16(&io_port->iop_paodr, PA_ENET_TXD);

	/* Configure port C pins to enable CLSN and RENA.
	 */
	clrbits16(&io_port->iop_pcpar, PC_ENET_CLSN | PC_ENET_RENA);
	clrbits16(&io_port->iop_pcdir, PC_ENET_CLSN | PC_ENET_RENA);
	setbits16(&io_port->iop_pcso, PC_ENET_CLSN | PC_ENET_RENA);

	/* Configure port A for TCLK and RCLK.
	 */
	setbits16(&io_port->iop_papar, PA_ENET_TCLK | PA_ENET_RCLK);
        clrbits16(&io_port->iop_padir, PA_ENET_TCLK | PA_ENET_RCLK);
        clrbits32(&cp->cp_pbpar, PB_ENET_TENA);
        clrbits32(&cp->cp_pbdir, PB_ENET_TENA);

	/* Configure Serial Interface clock routing.
	 * First, clear all SCC bits to zero, then set the ones we want.
	 */
	clrbits32(&cp->cp_sicr, SICR_ENET_MASK);
	setbits32(&cp->cp_sicr, SICR_ENET_CLKRT);

	/* In the original SCC enet driver the following code is placed at
	   the end of the initialization */
        setbits32(&cp->cp_pbpar, PB_ENET_TENA);
        setbits32(&cp->cp_pbdir, PB_ENET_TENA);

	immr_unmap(cp);
	immr_unmap(io_port);
}

void init_scc_ioports(struct fs_platform_info *fpi)
{
	int scc_no = fs_get_scc_index(fpi->fs_no);

	switch (scc_no) {
	case 2:
		init_scc3_ioports(fpi);
		break;
	default:
		printk(KERN_ERR "init_scc_ioports: invalid SCC number\n");
		return;
	}
}

static void init_smc1_uart_ioports(struct fs_uart_platform_info* ptr)
{
	cpm8xx_t *cp = (cpm8xx_t *)immr_map(im_cpm);

	setbits32(&cp->cp_pbpar, 0x000000c0);
	clrbits32(&cp->cp_pbdir, 0x000000c0);
	clrbits16(&cp->cp_pbodr, 0x00c0);
	immr_unmap(cp);

}

static void init_smc2_uart_ioports(struct fs_uart_platform_info* fpi)
{
	cpm8xx_t *cp = (cpm8xx_t *)immr_map(im_cpm);

	setbits32(&cp->cp_pbpar, 0x00000c00);
	clrbits32(&cp->cp_pbdir, 0x00000c00);
	clrbits16(&cp->cp_pbodr, 0x0c00);
	immr_unmap(cp);

}

void init_smc_ioports(struct fs_uart_platform_info *data)
{
	int smc_no = fs_uart_id_fsid2smc(data->fs_no);

	switch (smc_no) {
	case 0:
		init_smc1_uart_ioports(data);
		data->brg = data->clk_rx;
		break;
	case 1:
		init_smc2_uart_ioports(data);
		data->brg = data->clk_rx;
		break;
	default:
		printk(KERN_ERR "init_scc_ioports: invalid SCC number\n");
		return;
	}
}

int platform_device_skip(const char *model, int id)
{
	return 0;
}

static void __init mgsuvd_setup_arch(void)
{
	struct device_node *cpu;

	cpu = of_find_node_by_type(NULL, "cpu");
	if (cpu != 0) {
		const unsigned int *fp;

		fp = of_get_property(cpu, "clock-frequency", NULL);
		if (fp != 0)
			loops_per_jiffy = *fp / HZ;
		else
			loops_per_jiffy = 50000000 / HZ;
		of_node_put(cpu);
	}

	cpm_reset();

	mgsuvd_board_setup();

	ROOT_DEV = Root_NFS;
}

static struct of_device_id __initdata of_bus_ids[] = {
	{ .name = "soc", },
	{ .name = "cpm", },
	{ .name = "localbus", },
	{},
};

static int __init declare_of_platform_devices(void)
{
	if (!machine_is(mgsuvd))
		return 0;

	of_platform_bus_probe(NULL, of_bus_ids, NULL);

	return 0;
}
device_initcall(declare_of_platform_devices);

static int __init mgsuvd_probe(void)
{
	char *model = of_get_flat_dt_prop(of_get_flat_dt_root(),
					  "model", NULL);
	if (model == NULL)
		return 0;
	if (strcmp(model, "MGSUVD"))
		return 0;

	return 1;
}

define_machine(mgsuvd) {
	.name			= "MGSUVD",
	.probe			= mgsuvd_probe,
	.setup_arch		= mgsuvd_setup_arch,
	.init_IRQ		= m8xx_pic_init,
	.get_irq		= mpc8xx_get_irq,
	.restart		= mpc8xx_restart,
	.calibrate_decr		= mpc8xx_calibrate_decr,
	.set_rtc_time		= mpc8xx_set_rtc_time,
	.get_rtc_time		= mpc8xx_get_rtc_time,
};
