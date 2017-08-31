/*
 * arch/mips/include/asm/mach-au1x00/au1550_spi.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * au1550_spi.h - Au1550 PSC SPI controller driver - platform data structure
 */

#ifndef _AU1550_SPI_H_
#define _AU1550_SPI_H_

struct au1550_spi_info {
	u32 mainclk_hz;		/* main input clock frequency of PSC */
	u16 num_chipselect;	/* number of chipselects supported */
	void (*activate_cs)(struct au1550_spi_info *spi, int cs, int polarity);
	void (*deactivate_cs)(struct au1550_spi_info *spi, int cs, int polarity);
};

#endif
