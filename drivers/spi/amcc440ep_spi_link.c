/***************************************************************************
 *   Copyright (C) 2005 by J. Warren                                       *
 *   jeffw@synrad.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>

//#include <linux/at91_spi.h>
//#include <asm/arch/AT91RM9200_SPI.h>
//#include <asm/arch/at91_Flyer_Xilinx.h> //for the reads/writes to Xilinx
//#include <asm/arch/pio.h>
#include <linux/pci.h>


#define SPI_LINK_MAJOR 241
#define SPI_IOCTL_BASE 0xAF
#define SPI_GET_SERVO_STATUS _IOWR(SPI_IOCTL_BASE,1,int)
#define SPI_SET_SPI_MODE _IOR(SPI_IOCTL_BASE,2,int)
#define SPI_GET_BUFFER_SIZE _IOWR(SPI_IOCTL_BASE,5,int)
#define SPI_SERVO_RESET _IOR(SPI_IOCTL_BASE,6,int)
#define SPI_SERVO_BUS _IOR(SPI_IOCTL_BASE,7,int)
#define SPI_SERVO_READY _IOR(SPI_IOCTL_BASE,8,int)
#define SPI_SERVO_STATUS _IOR(SPI_IOCTL_BASE,9,int)
//make it big enough to accept the boot code currently 11000 bytes
#define BUFSIZE PAGE_SIZE*8

#ifdef DEBUG
#define MSG(string, args...) printk(KERN_DEBUG "spi_link:" string, ##args)
#else
#define MSG(string, args...)
#endif

static int iDev1 = 1;
static int iDev2 = 2;
static int bAcquiring = 0;
static int bAborted = 0;
DECLARE_WAIT_QUEUE_HEAD(wait_queue);
static int m_iStatus = 0;
char* readBuf;
char* writeBuf;
void* xil_addr;

#define SPI_MODE_MASK		(SPI_CPHA | SPI_CPOL | SPI_CS_HIGH \
				| SPI_LSB_FIRST |SPI_LOOP)

struct spidev_data {
	struct device		dev;
	struct spi_device	*spi;
	struct list_head	device_entry;

	struct mutex		buf_lock;
	unsigned		users;
	unsigned		curr_dev;
	u8			*buffer;
};

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

static unsigned bufsiz = 4096;

/* Allocate a single SPI transfer descriptor.  We're assuming that if multiple
   SPI transfers occur at the same time, spi_access_bus() will serialize them.
   If this is not valid, then either (i) each dataflash 'priv' structure
   needs it's own transfer descriptor, (ii) we lock this one, or (iii) use
   another mechanism.   */
//static struct spi_transfer_list* spi_transfer_desc;
/*
 * Perform a SPI transfer to access the DataFlash device.
 */
static int do_spi_transfer(int nr, char* tx, int tx_len, char* rx, int rx_len,
		char* txnext, int txnext_len, char* rxnext, int rxnext_len, int txtype)
{
	struct spi_transfer_list* list = spi_transfer_desc;

	list->tx[0] = tx;	list->txlen[0] = tx_len;
	list->rx[0] = rx;	list->rxlen[0] = rx_len;

	list->tx[1] = txnext; 	list->txlen[1] = txnext_len;
	list->rx[1] = rxnext;	list->rxlen[1] = rxnext_len;

	list->nr_transfers = nr;
	list->txtype = txtype;
	return spi_transfer(list);
}

//read is used for the asynchronous write
static ssize_t spi_link_read(struct file* file, char* buf, size_t count, loff_t *offset)
{
    ssize_t ret;    
    int type = SPI_ASYNC;    
    int *pMinor = (int*)file->private_data;
    if (*pMinor != iDev1)
	return 0;     
    unsigned short xil;
    
    int writeCount =(count > BUFSIZE ? BUFSIZE : count);
    if (spi_ready(*pMinor) == -1)
    {
	//this is bad...
	printk(KERN_ERR "%d pid tried to read while SPI bus not acquired!\n",current->pid);
	return -1;
    }
    
    copy_from_user(writeBuf,buf,writeCount);
    if(!xil_addr)
	xil_addr = at91_xil_get_mapped_address();
    xil = *((unsigned short*)xil_addr + XIL_STATUS_OFFSET);
    m_iStatus = ((int)xil << 16) + (spi_transfer_desc->servo_rxval & 0xffff);
    do_spi_transfer(1,writeBuf,writeCount,readBuf,writeCount,NULL,0,NULL,0,type);    
    
    
    ret = (ssize_t)( (int)xil << 16);
    return ret;
}

//write is ALWAYS synchronous
static ssize_t spi_link_write(struct file* file, const char* buf, size_t count, loff_t *offset)
{
    ssize_t ret;
    int type = SPI_SYNC;    
    int *pMinor = (int*)file->private_data;
    //if (*pMinor != iDev2 || *pMinor != iDev3)
	//return 0;
    unsigned short xil;
    int releasebus = 0;
    int writeCount =(count > BUFSIZE ? BUFSIZE : count);
    if(!xil_addr)
	xil_addr = at91_xil_get_mapped_address();
    if (spi_ready(*pMinor)==-1)
    {

        if (bAcquiring)
	{
	    wait_queue_t wait;
	    init_waitqueue_entry(&wait,current);
	    add_wait_queue(&wait_queue,&wait);
	    set_current_state(TASK_INTERRUPTIBLE);
	    if (bAcquiring)
	    {
		printk(KERN_ERR "%d:bAcquiring\n",current->pid);
		schedule();//put the calling process to sleep
	    }
	    set_current_state(TASK_RUNNING);
	    remove_wait_queue(&wait_queue,&wait);	    
	    if (bAborted)
	    {
		printk(KERN_ERR "spi_link_write: pid:%d, Aborted\n",current->pid);
		bAborted = 0;
		return -1;
	    }
	}
	else
	{
	    spi_access_bus(*pMinor);
	    releasebus = 1;
	}
	
    }
    copy_from_user(writeBuf,buf,writeCount);
    
    while(!spi_ready(*pMinor));
    
    do_spi_transfer(1,writeBuf,writeCount,readBuf,writeCount,NULL,0,NULL,0,type);
    
    if (releasebus)
    {	
	spi_release_bus(*pMinor);	
    }
    
    xil = *((unsigned short*)xil_addr + XIL_STATUS_OFFSET);
    ret = (ssize_t)( ((int)xil << 16) + ((unsigned short*)readBuf)[writeCount/2 -1]);
    m_iStatus = ret;
    return ret;
}

static int spi_link_open(struct inode* inode, struct file* file)
{
	int iDevCurrent = iminor(inode);
	MSG("Module spi_link open, iMinor = %d\n",iDevCurrent );
	if (iDevCurrent < 2 || iDevCurrent > 3)
		return -ENODEV;
	if (iDevCurrent == 2)
		file->private_data = &iDev2;
	else if (iDevCurrent ==3)
		file->private_data = &iDev3;
	return 0;
}

static int spi_link_release(struct inode* inode, struct file* file)
{
    int iMinor = iminor(inode);
    if (spi_ready(iMinor)!=-1)
    {	
	spi_release_bus(iMinor);		
    }	
    if (waitqueue_active(&wait_queue))
    {	
	bAborted = 1;
	wake_up_interruptible(&wait_queue);
	printk(KERN_ERR "Found process on wait_queue during releasebus!\n");
    }
    return 0;
}

static int spi_link_ioctl(struct inode* inode, struct file* file, unsigned int cmd, unsigned long arg)
{
    AT91PS_SPI controller = (AT91PS_SPI) AT91C_VA_BASE_SPI;
    
    short* pBuf = (short*)0;
    int iMinor = iminor(inode);
    short* pReadBuf = (short*)readBuf;
    int bufSize = 0;
    int i=0;
	/* Make sure the command belongs to us*/
    if (_IOC_TYPE(cmd) != SPI_IOCTL_BASE)
    	return -ENOTTY;
    
    switch(cmd )
    {
    case SPI_GET_SERVO_STATUS:
	//the buffer size is passed in first, then the buffer location
	//the buffer size gives us the count of bytes sent which we can
	//then deduce the word that we want to send back.  We want to only
	//send back the last status.
	bufSize = *(int*)arg;
	pBuf = (short*) *(int*)(arg + sizeof(int));
	MSG("iMinor:%d, pReadBuf:0x%x, bufSize:%d\n",iMinor,(int)pReadBuf,bufSize);
	if (bufSize > BUFSIZE)
	    bufSize = BUFSIZE;
	i = copy_to_user(pBuf,&pReadBuf[(bufSize/2) -1],sizeof(short));
    	return sizeof(short) - i;
	break;
    case SPI_SET_SPI_MODE:
	//Set the bit mode to 8 or 16 bit, polarity,phase and baud rate
	//The calling application will have to know how to fill out this register.
	if (iMinor == 2)
	{	    	
	    controller->SPI_CSR2 = arg;//servo
	}
	else if (iMinor == 3)
	{
	    controller->SPI_CSR3 = arg;//expansion slot
	}
	return arg;
	break;
    case SPI_GET_BUFFER_SIZE:
	return BUFSIZE;
	break;
    case SPI_SERVO_RESET:
	AT91_SYS->PIOD_PER = 1 << 8; //line 8
	AT91_SYS->PIOD_OER = 1 << 8; //set it as output, we want to drive it
	
	AT91_SYS->PIOD_SODR = 1 << 8;//set it high initially;
	//disable glitch filters
	AT91_SYS->PIOD_IFDR = 1 << 8;
	//disable interrupts
	AT91_SYS->PIOD_IDR = 1 << 8;
	//disable multiple drivers
	AT91_SYS->PIOD_MDDR = 1 << 8;
	//disable internal pull-up
	AT91_SYS->PIOD_PPUDR = 1 << 8; 
	
	//here is the actual reset
	AT91_SYS->PIOD_CODR = 1 << 8;
	//give it enough time so that it will hit the reset.
	nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
	nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
	nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
	nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
	AT91_SYS->PIOD_SODR = 1 << 8;
	return 1;
	break;
    case SPI_SERVO_BUS:
	if (arg)//grabbing the bus
	{
	    if (spi_ready(iMinor)==-1)
	    {		
		bAcquiring = 1;
		spi_access_bus(iMinor);		
		bAcquiring = 0;		
		wake_up_interruptible(&wait_queue);
	    }
	}
	else //releasing the bus
	{
	    if (spi_ready(iMinor)!=-1)
	    {
		spi_release_bus(iMinor);			
	    }	
	    if (waitqueue_active(&wait_queue))
	    {	
		bAborted = 1;
		wake_up_interruptible(&wait_queue);
		printk(KERN_ERR "Found process on wait_queue during releasebus!\n");
	    }
	}
	break;
    case SPI_SERVO_READY:
	return spi_ready(iMinor);
	break;
    case SPI_SERVO_STATUS:
	return m_iStatus;
	break;
    default:
    	return -ENOTTY;    	
    }
    return 0;
}

/********************************************************************/
static struct file_operations spi_link_fops = {
owner:		THIS_MODULE,
read:		spi_link_read,
write:		spi_link_write,
ioctl:		spi_link_ioctl,
open:		spi_link_open,
release:	spi_link_release,
};

static int __exit
amcc440ep_spi_dev_remove(struct spi_device *spi)
{
printk(KERN_INFO "%s\n", __FUNCTION__);
	//destroy_workqueue(workqueue);
	return 0;
}

static struct spi_driver amcc440ep_spidev = {
         .driver = {
                 .name =         "amcc440ep_spidev",
                 .bus =          &spi_bus_type,
                .owner =        THIS_MODULE,
         },
         .probe =        spi_ppc4xx_test_probe,
         .remove =       __devexit_p(amcc440ep_spi_dev_remove),
 };

static int __init at91_spi_link_init_module(void)
{
	int res = 0;
	xil_addr = NULL;
	MSG("Module at91_spi_link init\n" );		
	readBuf = kmalloc(BUFSIZE, GFP_KERNEL);
	writeBuf = kmalloc(BUFSIZE, GFP_KERNEL);
	memset(readBuf,0,BUFSIZE);//clear the read buffer
	printk(KERN_DEBUG "readBuf:0x%x, writeBuf:0x%x\n",(int)readBuf,(int)writeBuf);
	/*register the device with the kernel*/	
	res = register_chrdev(SPI_LINK_MAJOR,"spi_link",&spi_link_fops);
	if (res)
	{
		MSG("Can't register device spi_link with kernel.\n");
		return res;
	}
	
	res = spi_register_driver(&amcc440ep_spidev);
	if (res < 0)
	{
		unregister_chrdev(SPI_LINK_MAJOR,"spi_link");
		return res;
	}
	
	return 0;
}

static void __exit at91_spi_link_exit_module(void)
{
        int res;
	printk( KERN_DEBUG "Module at91_spi_link exit\n" );
	
	if (readBuf)
	{
		kfree(readBuf);
	}
	if (writeBuf)
	{
		kfree(writeBuf);
	}
	
	res = spi_unregister_driver(&amcc440ep_spidev);
	/*unregister the device with the kernel*/	
	res = unregister_chrdev(SPI_LINK_MAJOR,"spi_link");
	if (res)
	{
		MSG("Can't unregister device spi_link with kernel.\n");
	}
}

module_init(at91_spi_link_init_module);
module_exit(at91_spi_link_exit_module);


MODULE_DESCRIPTION("Module to communicate with the spi peripherals");
MODULE_AUTHOR("J. Warren (jeffw@synrad.com)");
MODULE_LICENSE("GPL");
