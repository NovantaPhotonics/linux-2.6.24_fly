/*
 * spi_ppc4xx_test.c
 *
 * This is a one-off driver to test the spi_ppc4xx.c master SPI driver
 * and is very specific to one modified development board. It is not
 * suited for any other hardware or to any other purpose!
 *
 * Copyright (C) Gary Jennejohn <garyj@denx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 , the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/errno.h>
#ifdef CONFIG_PPC_OF
#include <asm/of_platform.h>
#else
#include <linux/platform_device.h>
#endif
#include <linux/spi/spi.h>

struct spippc4xx_spi;
/* to schedule the xfer after the init */
struct workqueue_struct	*workqueue;
struct work_struct	work;
struct spi_device *save_spi;

/*
 * This is a little peculiar in half-duplex because:
 * (a) CS 45 is used for writes and rx_buf is always NULL
 * (b) CS 44 is used for reads and tx_buf is always NULL
 * (c) cannot use spi_message because the driver has to change the
 *    chipselect between read and write
 * Basically the controller is treated as a half-duplex device.
 * The hardware design of the test hardware makes that necessary.
 *
 * The easiest way to do things is to call spi_write() and spi_read().
 */
static void
spi_ppc4xx_test_xfer(struct work_struct *work)
{
	u8 buf[8];
	int res;
	struct spippc4xx_spi *hw;

	hw = (struct spippc4xx_spi *)spi_master_get_devdata(save_spi->master);
	save_spi->chip_select = 1;
	spi_setup(save_spi);
	/*
	 * the default setup:
	 * spi->mode = SPI_MODE_0;
	 * spi->chip_select = 45;
	 * spi->max_speed_hz = 10000000;
	 */

	buf[0] = 0x06;
	buf[1] = 0x20;
	buf[2] = 0x55;
	buf[3] = 0xaa;
	buf[4] = 0x06;
	buf[5] = 0x7f;
	buf[6] = 0x55;
	buf[7] = 0xaa;
	res = spi_write(save_spi, buf, 8);
	//if (res == 0)
	 	//printk("wrote %x %x\n", buf[0], buf[1]);
	save_spi->chip_select = 1;
	spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}
	buf[0] = 0x55;
	buf[1] = 0xaa;
	res = spi_read(save_spi, buf, 2);
	if (res == 0)
	 	printk("read %x %x\n", buf[0], buf[1]);

	save_spi->chip_select = 1;
	res = spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}

	buf[0] = 0x06;
	buf[1] = 0x7f;
	res = spi_write(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "wrote %x %x\n", buf[0], buf[1]);
	save_spi->chip_select = 1;
	spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}
	buf[0] = 0x55;
	buf[1] = 0xaa;
	res = spi_read(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "read %x %x\n", buf[0], buf[1]);

	save_spi->chip_select = 1;
	res = spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}

	buf[0] = 0x06;
	buf[1] = 0xaf;
	res = spi_write(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "wrote %x %x\n", buf[0], buf[1]);
	save_spi->chip_select = 1;
	spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}
	buf[0] = 0x55;
	buf[1] = 0xaa;
	res = spi_read(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "read %x %x\n", buf[0], buf[1]);

	save_spi->chip_select = 1;
	res = spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}

	buf[0] = 0x06;
	buf[1] = 0xff;
	res = spi_write(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "wrote %x %x\n", buf[0], buf[1]);

	 /* need to change the chip select for the read */
	save_spi->chip_select = 1;
	spi_setup(save_spi);
	if (res < 0) {
		dev_dbg(&save_spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				save_spi->mode, save_spi->max_speed_hz / 1000,
				res);
		return;
	}
	buf[0] = 0x55;
	buf[1] = 0xaa;
	res = spi_read(save_spi, buf, 2);
	if (res == 0)
	 	printk(KERN_INFO "read %x %x\n", buf[0], buf[1]);
}

static int __init
spi_ppc4xx_test_probe(struct spi_device *spi)
{
	int res;

        printk("%s master %p\n", __FUNCTION__, spi->master);

	save_spi = spi;
	spi->mode = SPI_MODE_0;
	spi->chip_select = 0;

	res = spi_setup(spi);
	if (res < 0) {
		dev_dbg(&spi->dev, "needs SPI mode %02x, %d KHz; %d\n",
				spi->mode, spi->max_speed_hz / 1000,
				res);
		return res;
	}
	INIT_WORK(&work, spi_ppc4xx_test_xfer);
	workqueue = create_singlethread_workqueue("xfer");
	if (workqueue == NULL)
		return -EBUSY;

	queue_work(workqueue, &work);

	return 0;
}

static int __exit
spi_ppc4xx_test_remove(struct spi_device *spi)
{
printk(KERN_INFO "%s\n", __FUNCTION__);
	destroy_workqueue(workqueue);
	return 0;
}

static struct spi_driver spi_ppc4xx_test_driver = {
         .driver = {
                 .name =         "spi_ppc4xx_test",
                 .bus =          &spi_bus_type,
                .owner =        THIS_MODULE,
         },
         .probe =        spi_ppc4xx_test_probe,
         .remove =       __devexit_p(spi_ppc4xx_test_remove),
 };

static int __init
spi_ppc4xx_test_init(void)
{
        int status;
    
        printk("%s\n", __FUNCTION__);
	status = spi_register_driver(&spi_ppc4xx_test_driver);
	if(status < 0)
	    printk("%s: Failed to register spi driver\n", __FUNCTION__);
	return status;
}
module_init(spi_ppc4xx_test_init);


static void __exit
spi_ppc4xx_test_exit(void)
{
printk(KERN_INFO "%s\n", __FUNCTION__);
	spi_unregister_driver(&spi_ppc4xx_test_driver);
}
module_exit(spi_ppc4xx_test_exit);

MODULE_LICENSE("GPL");
