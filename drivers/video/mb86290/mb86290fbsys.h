/*
 *	mb86290fbsys.h 	--  MB86290 Series FrameBuffer Driver
 *
 *      Copyright (C) FUJITSU LIMITED 2003
 *	1.01.002
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
#ifndef _MB86290FBSYS_H_
#define _MB86290FBSYS_H_

/*---------------------------*/
/* User definition parameter */
/*---------------------------*/
/* common */
#if !defined(CONFIG_FB_MB86290_LIME)
#define	MB86290FB_USE_PCI		1
#endif
#define	MB86290FB_USE_EEPROM		0
#define	MB86290FB_USE_BCUCTL		0

/* Burst Setup Register(MB86295) */
#define	MB86290FB_BSR_MASK		0

#ifdef CORAL_LP
#define	MB86290FB_DFLT_PCI_VENDOR_ID	MB86290FB_PCI_VENDOR_ID_FUJITSU
#define	MB86290FB_DFLT_PCI_DEVICE_ID	MB86290FB_PCI_DEVICE_ID_MB86295
#define	MB86290FB_PCI_BASEADDRESS	0
#define MB86290FB_DFLT_IP_GDCTYPE	MB86290FB_TYPE_MB86295
#define	MB86290FB_TRAN_UNIT		32
#elif defined(CORAL_LB)
#define	MB86290FB_DFLT_PCI_VENDOR_ID	MB86290FB_PCI_VENDOR_ID_PLX
#define	MB86290FB_DFLT_PCI_DEVICE_ID	MB86290FB_PCI_DEVICE_ID_MB86292EV
#define	MB86290FB_PCI_BASEADDRESS	2
#define MB86290FB_DFLT_IP_GDCTYPE	MB86290FB_TYPE_MB86294
#define	MB86290FB_TRAN_UNIT		4
#elif defined(LIME)
#define MB86290FB_DFLT_IP_GDCTYPE	MB86290FB_TYPE_MB86296
#define	MB86290FB_TRAN_UNIT		4
#else
#define	MB86290FB_DFLT_PCI_VENDOR_ID	MB86290FB_PCI_VENDOR_ID_PLX
#define	MB86290FB_DFLT_PCI_DEVICE_ID	MB86290FB_PCI_DEVICE_ID_MB86292EV
#define	MB86290FB_PCI_BASEADDRESS	2
#define MB86290FB_DFLT_IP_GDCTYPE	MB86290FB_TYPE_MB86292
#define	MB86290FB_TRAN_UNIT		4
#endif

/* DMA Memory */
#define	MB86290FB_DMAMEM_GFP
#define MB86290FB_DMAMEM_PHYSADDR	0x7E00000
#define MB86290FB_DMAMEM_SIZE		7
#define MB86290FB_DMAMEM_SIZE_BYTE	(512*1024)
#define	MB86290FB_DMAMEM_MAXSEGMENT	16

/* VRAM */
#define MB86290FB_VRAM_SIZE		0x2000000 /* 32MB */
#define	MB86290FB_VRAM_MAXSEGMENT	32

/* GDC initialize parameter */
#define MB86290FB_DFLT_IP_CPUTYPE	MB86290FB_CPU_x86

#if defined(LIME)
#define MB86290FB_DFLT_IP_GDCBASE	0xC0000000UL
#define MB86290FB_DFLT_IP_GEOCLOCK	0x0
#define MB86290FB_DFLT_IP_OTHERCLOCK	MB86290FB_CLOCK_100MHZ
#else
#define MB86290FB_DFLT_IP_GDCBASE	0x00000000UL
#define MB86290FB_DFLT_IP_GEOCLOCK	MB86290FB_CLOCK_166MHZ
#define MB86290FB_DFLT_IP_OTHERCLOCK	MB86290FB_CLOCK_133MHZ
#endif

#define MB86290FB_DFLT_IP_LOCATE	MB86290FB_REG_LOCATE_CENTER

#ifdef CORAL_LP
#define MB86290FB_DFLT_IP_MEMORYMODE	0x11d7fa13
#elif defined(CORAL_LB)
#define MB86290FB_DFLT_IP_MEMORYMODE	0x014EB813
#elif defined(LIME)
#if 0
 #define MB86290FB_DFLT_IP_MEMORYMODE	0x414FB7F3 /* 133MHz, CL3 ..b473? */
#else
 #define MB86290FB_DFLT_IP_MEMORYMODE	0x414FB7F2 /* 100MHz, CL2 */
#endif
#else
#define MB86290FB_DFLT_IP_MEMORYMODE	0x009a984a
#endif

/* Screen information of frame buffer */
#ifdef CONFIG_FB_MB86290_640X480_16BPP
#define	MB86290FB_DFLT_SI_XRES			640
#define	MB86290FB_DFLT_SI_YRES			480
#define	MB86290FB_DFLT_SI_XRES_VIRTUAL		640
#define	MB86290FB_DFLT_SI_YRES_VIRTUAL		480
#else
#define MB86290FB_DFLT_SI_XRES                  1024
#define MB86290FB_DFLT_SI_YRES                  768
#define MB86290FB_DFLT_SI_XRES_VIRTUAL          1024
#define MB86290FB_DFLT_SI_YRES_VIRTUAL          768
#endif
#define	MB86290FB_DFLT_SI_XOFFSET		0
#define	MB86290FB_DFLT_SI_YOFFSET		0
#define	MB86290FB_DFLT_SI_BITS_PER_PIXEL	16
#define	MB86290FB_DFLT_SI_GRAYSCALE		0
#define	MB86290FB_DFLT_SI_RED_OFFSET		10
#define	MB86290FB_DFLT_SI_RED_LENGTH		5
#define	MB86290FB_DFLT_SI_RED_RIGHT		0
#define	MB86290FB_DFLT_SI_GREEN_OFFSET		5
#define	MB86290FB_DFLT_SI_GREEN_LENGTH		5
#define	MB86290FB_DFLT_SI_GREEN_RIGHT		0
#define	MB86290FB_DFLT_SI_BLUE_OFFSET		0
#define	MB86290FB_DFLT_SI_BLUE_LENGTH		5
#define	MB86290FB_DFLT_SI_BLUE_RIGHT		0
#define	MB86290FB_DFLT_SI_TRANSP_OFFSET		0
#define	MB86290FB_DFLT_SI_TRANSP_LENGTH		1
#define	MB86290FB_DFLT_SI_TRANSP_RIGHT		0
#define	MB86290FB_DFLT_SI_NONSTD		0
#define	MB86290FB_DFLT_SI_ACTIVATE		FB_ACTIVATE_NOW
#define	MB86290FB_DFLT_SI_HEIGHT		-1
#define	MB86290FB_DFLT_SI_WIDTH			-1
#define	MB86290FB_DFLT_SI_ACCEL_FLAGS		0
#define	MB86290FB_DFLT_SI_PIXCLOCK		0
#define	MB86290FB_DFLT_SI_LEFT_MARGIN		0
#define	MB86290FB_DFLT_SI_RIGHT_MARGIN		0
#define	MB86290FB_DFLT_SI_UPPER_MARGIN		0
#define	MB86290FB_DFLT_SI_LOWER_MARGIN		0
#define	MB86290FB_DFLT_SI_HSYNC_LEN		0
#define	MB86290FB_DFLT_SI_VSYNC_LEN		0
#define	MB86290FB_DFLT_SI_SYNC			0
#define	MB86290FB_DFLT_SI_VMODE			FB_VMODE_NONINTERLACED
#define	MB86290FB_DFLT_SI_RESERVED		{ 0,0,0,0,0,0 }

/* Timing Parameter */
#if ((MB86290FB_DFLT_SI_XRES == 640) && (MB86290FB_DFLT_SI_YRES == 480))
#define MB86290FB_DFLT_IP_MODE		0x0F00
#define MB86290FB_DFLT_IP_HTP		800
#define MB86290FB_DFLT_IP_HSP		656
#define MB86290FB_DFLT_IP_HSW		96
#define MB86290FB_DFLT_IP_HDP		640
#define MB86290FB_DFLT_IP_VTR		525
#define MB86290FB_DFLT_IP_VSP		491
#define MB86290FB_DFLT_IP_VSW		2
#define MB86290FB_DFLT_IP_VDP		480
#elif ((MB86290FB_DFLT_SI_XRES == 1024) && (MB86290FB_DFLT_SI_YRES == 768))
#define MB86290FB_DFLT_IP_MODE          0x0500
#define MB86290FB_DFLT_IP_HTP           1344
#define MB86290FB_DFLT_IP_HSP           1032
#define MB86290FB_DFLT_IP_HSW           144
#define MB86290FB_DFLT_IP_HDP           1024
#define MB86290FB_DFLT_IP_VTR           806
#define MB86290FB_DFLT_IP_VSP           771
#define MB86290FB_DFLT_IP_VSW           6
#define MB86290FB_DFLT_IP_VDP           768
#endif

/* Event flag */
#define MB86290FB_EVENTWAIT_MAX		32

#endif /*_MB86290FBSYS_H_*/
