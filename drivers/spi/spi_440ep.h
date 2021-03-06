/*
 * Register definitions for Synrad Serial Peripheral Interface (SPI)
 *
 * Copyright (C) 2008 Synrad Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __SPI_440EP_H__
#define __SPI_440EP_H__

#define SPI_IRQ_START		0x0C2

/*SPI Interrupt Register */
struct amcc440ep_spi_irq_reg {
    __be32 enable;
};

/* SPI Registers */

/* SPI Controller registers */
struct amcc440ep_spi_reg {
	u8 mode;
	u8 RxD;
	u8 TxD;
	u8 control;
	u8 status;
	u8 pad;
	u8 clock;
};

/* SPI Controller mode register definitions */
#define	SPMODE_LOOP		0x1
#define	SPMODE_CI_ACTIVELOW	0x2
#define	SPMODE_MSB		0x4
#define	SPMODE_ENABLE		0x8
#define	SPMODE_CP_LEADING_EDGE	0x10

/* SPI Status Register definitions */
#define SPSTATUS_RXREADY	0x1
#define SPSTATUS_BUSY		0x2

/*SPI Control Register Definitions */
#define SPCTRL_STR_ENABLE	0x1

/* clock settings (SCP and CI) for various SPI modes */
#define SPI_CLK_MODE0      SPMODE_CP_LEADING_EDGE
#define SPI_CLK_MODE1      0
#define SPI_CLK_MODE2      SPMODE_CI_ACTIVELOW
#define SPI_CLK_MODE3      (SPMODE_CP_LEADING_EDGE|SPMODE_CI_ACTIVELOW)


/* SPI Clock Divisor Register */
// SPI Clock Out = OPBCLK/(4*(CDM +1)) where CDM = 0-255

#define SPI_MODE	0x0
#define SPI_RXD		0x1
#define SPI_TXD		0x2
#define SPI_CR		0x3
#define SPI_SR		0x4
#define SPI_CLK		0x6


/* GPIO Registers */
/*
#define GPIO_PHYS_START		0x0EF600B00
#define GPIO_PHYS_END		0x0EF600B44	

struct amcc440ep_gpio_reg {
    __be32 orr;				// Output Register		        RW
    __be32 tcr;				// Tri-State Control Register		RW
    __be32 osrl;			// Output Select Register Low		RW
    __be32 osrh;			// Output Select Register Hi		RW
    __be32 tsrl;			// Tri-State Select Register Low	RW
    __be32 tsrh;			// Tri-State Select Register HI		RW
    __be32 odr;				// Open Drain Register			RW
    __be32 ir;				// Input register			R
    __be32 rr1;				// Receive Register 1			RW
    __be32 rr2;				// Receive Register 2			RW
    __be16 rr3;				// Receive Register 3			RW
    __be32 isr1l;			// Input Select Register 1 Low		RW
    __be32 isr1h;			// Input Select Register 1 Hi		RW
    __be32 isr2l;			// Input Select Register 2 Low		RW
    __be32 isr2h;			// Input Select Register 2 Hi		RW
    __be32 isr3l;			// Input Select Register 3 Low		RW
    __be32 isr3h;			// Input Select Register 3 Hi		RW
};

//GPIO Register definitions //

#define SIGNAL_CTRL(x)	   ((x) << 30)
#define SET_BIT32(x)	   (1 << (31-x))
*/

#define SPI_SYNC	0
#define SPI_ASYNC	1

#endif /* __SPI_440EP_H__ */
