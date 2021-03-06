/*
 * arch/ppc/platforms/4xx/yosemite.h
 *
 * Yosemite and Yellowstone board definitions
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 *
 * Copyright 2004 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifdef __KERNEL__
#ifndef __ASM_FLYER_H__
#define __ASM_FLYER_H__

#include <platforms/4xx/ibm440ep.h>

/* Default clock rate */
#define FLYER_TMRCLK                        50000000
#define FLYER_SYSCLK                        66666666

/*
 * Serial port defines
 */
#define RS_TABLE_SIZE                  2

#define UART0_IO_BASE                  0xEF600300
#define UART1_IO_BASE                  0xEF600400

#define BASE_BAUD                      33177600/3/16
#define UART0_INT                      0
#define UART1_INT                      1

#define STD_UART_OP(num)                                       \
       { 0, BASE_BAUD, 0, UART##num##_INT,                     \
               (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST),        \
               iomem_base: UART##num##_IO_BASE,                \
               io_type: SERIAL_IO_MEM},

#define SERIAL_PORT_DFNS       \
       STD_UART_OP(0)          \
       STD_UART_OP(1)

/* RTC/NVRAM location */
#define FLYER_RTC_ADDR			0x0fb100000ULL
#define FLYER_RTC_SIZE			0x8000

/* FPGA Registers */
#define BAMBOO_FPGA_ADDR		0x0fb000000ULL

/* PCI support */
#define FLYER_PCI_IO_BASE           0x00000000e8000000ULL
#define FLYER_PCI_IO_SIZE           0x00010000
#define FLYER_PCI_MEM_OFFSET        0x00000000
#define FLYER_PCI_PHY_MEM_BASE      0x00000000a0000000ULL

#define FLYER_PCI_LOWER_IO          0x00000000
#define FLYER_PCI_UPPER_IO          0x0000ffff
#define FLYER_PCI_LOWER_MEM         0xa0000000
#define FLYER_PCI_UPPER_MEM         0xafffffff
#define FLYER_PCI_MEM_BASE          0xa0000000

#define FLYER_PCIL0_BASE            0x00000000ef400000ULL
#define FLYER_PCIL0_SIZE            0x40

#define FLYER_PCIL0_PMM0LA          0x000
#define FLYER_PCIL0_PMM0MA          0x004
#define FLYER_PCIL0_PMM0PCILA       0x008
#define FLYER_PCIL0_PMM0PCIHA       0x00C
#define FLYER_PCIL0_PMM1LA          0x010
#define FLYER_PCIL0_PMM1MA          0x014
#define FLYER_PCIL0_PMM1PCILA       0x018
#define FLYER_PCIL0_PMM1PCIHA       0x01C
#define FLYER_PCIL0_PMM2LA          0x020
#define FLYER_PCIL0_PMM2MA          0x024
#define FLYER_PCIL0_PMM2PCILA       0x028
#define FLYER_PCIL0_PMM2PCIHA       0x02C
#define FLYER_PCIL0_PTM1MS          0x030
#define FLYER_PCIL0_PTM1LA          0x034
#define FLYER_PCIL0_PTM2MS          0x038
#define FLYER_PCIL0_PTM2LA          0x03C

#endif                          /* __ASM_FLYER_H__ */
#endif                          /* __KERNEL__ */
