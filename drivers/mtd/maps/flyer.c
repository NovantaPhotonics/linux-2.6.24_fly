/*
 *
 * drivers/mtd/maps/taihu.c
 *
 * FLASH map for the Synrad Flyer II board.
 *
 * 2010 Synrad, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>

#define RW_PART0_OF	0
#define RW_PART0_SZ	0x180000
#define RW_PART1_SZ	0x280000
/* Partition 2 will be autosized dynamically... */
//#define RW_PART3_SZ     0x40000
//#define RW_PART4_SZ	  0x80000
#define RW_PART3_SZ	0x20000
#define RW_PART4_SZ	0x80000
//#define RW_PART5_SZ	0x01000000

#define OSWINDOW_ADDR 0xfe000000
#define OSWINDOW_SIZE 0x02000000

#define FSWINDOW_ADDR 	0xfc000000
#define FSWINDOW_SIZE 	0x02000000


static struct mtd_partition flyer_osflash_partitions[] = {
    {
    .name = "Kernel",
   .offset = 0,
   .size = RW_PART0_SZ
		    },
      {
	  .name = "OS Filesystem",
   .offset = MTDPART_OFS_APPEND,
   /*		.size = RW_PART2_SZ */ /* will be adjusted dynamically */
      },
      {
	  .name = "U-Boot Env",
   .offset = MTDPART_OFS_APPEND,
   .size = RW_PART3_SZ,
      },
      {
	  .name = "U-Boot",
   .offset = MTDPART_OFS_APPEND,
   .size = RW_PART4_SZ,
      }
};

struct map_info flyer_osflash_map = {
	.name = "Synrad Flyer II OS Flash",
	.size = OSWINDOW_SIZE,
	.bankwidth = 2,
	.phys = OSWINDOW_ADDR,
};

static struct mtd_partition flyer_fsflash_partitions[] = {
	{
		.name = "User Files",
		.offset = 0,
		.size = OSWINDOW_SIZE,
	}
};


struct map_info flyer_fsflash_map = {
	.name = "Synrad Flyer II Filestore Flash",
	.size = FSWINDOW_SIZE,
	.bankwidth = 2,
	.phys = FSWINDOW_ADDR,
};

#define NUM_FLYER_FLASH_PARTITIONS(parts)	\
	(sizeof(parts)/sizeof(parts[0]))

static struct mtd_info *flyer_mtd;
//static struct mtd_info *flyer_mtd2;

int __init init_flyer_flash(void)
{

	printk(KERN_NOTICE "Flyer II: OS flash mapping: %x at %x\n",
	       OSWINDOW_SIZE, OSWINDOW_ADDR);
	/*
	* Adjust partition 1 to flash size
	*/
	flyer_osflash_partitions[1].size = OSWINDOW_SIZE -RW_PART0_SZ - RW_PART3_SZ - RW_PART4_SZ;
	flyer_osflash_map.virt = ioremap(OSWINDOW_ADDR, OSWINDOW_SIZE);
	if (!flyer_osflash_map.virt) {
	    printk("init_Flyer_flash: failed to ioremap for OS flash\n");
		return -EIO;
	}
	simple_map_init(&flyer_osflash_map);
	flyer_mtd = do_map_probe("cfi_probe", &flyer_osflash_map);
	if (flyer_mtd) {
		flyer_mtd->owner = THIS_MODULE;
		//add_mtd_device(flyer_mtd);
		add_mtd_partitions(flyer_mtd,
				   flyer_osflash_partitions,
				   ARRAY_SIZE(flyer_osflash_partitions));
	} else {
	    printk("map probe failed (OS flash)\n");
		return -ENXIO;
	}
	
	printk(KERN_NOTICE "Flyer II: Filestore mapping: %x at %x\n",
	       FSWINDOW_SIZE, FSWINDOW_ADDR);
	flyer_fsflash_map.virt = ioremap(FSWINDOW_ADDR, FSWINDOW_SIZE);
	if (!flyer_fsflash_map.virt) {
		printk("init_Flyer_flash: failed to ioremap for Filestore\n");
		return -EIO;
	}
	simple_map_init(&flyer_fsflash_map);
	flyer_mtd = do_map_probe("cfi_probe", &flyer_fsflash_map);
	if (flyer_mtd) {
		flyer_mtd->owner = THIS_MODULE;
		//add_mtd_device(flyer_mtd);
		add_mtd_partitions(flyer_mtd,
				   flyer_fsflash_partitions,
				   ARRAY_SIZE(flyer_fsflash_partitions));

	} else {
		printk("map probe failed (Filestore Flash)\n");
		return -ENXIO;
	}
	
    
	return 0;
}

static void __exit cleanup_flyer_flash(void)
{
	if (flyer_mtd) {
		del_mtd_partitions(flyer_mtd);
		/* moved iounmap after map_destroy - armin */
		map_destroy(flyer_mtd);
	}
	
	if (flyer_osflash_map.virt)
		iounmap((void *)flyer_osflash_map.virt);
	
	if (flyer_fsflash_map.virt)
		iounmap((void *)flyer_fsflash_map.virt);
	
}

module_init(init_flyer_flash);
module_exit(cleanup_flyer_flash);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD map driver for the Synrad Flyer II board");
