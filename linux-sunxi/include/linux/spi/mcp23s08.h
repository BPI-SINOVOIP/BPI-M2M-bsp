/*
 * include/linux/spi/mcp23s08.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

/* FIXME driver should be able to handle IRQs...  */

struct mcp23s08_chip_info {
	bool		is_present;	/* true if populated */
	unsigned	pullups;	/* BIT(x) means enable pullup x */
};

struct mcp23s08_platform_data {
	/* For mcp23s08, up to 4 slaves (numbered 0..3) can share one SPI
	 * chipselect, each providing 1 gpio_chip instance with 8 gpios.
	 * For mpc23s17, up to 8 slaves (numbered 0..7) can share one SPI
	 * chipselect, each providing 1 gpio_chip (port A + port B) with
	 * 16 gpios.
	 */
	struct mcp23s08_chip_info	chip[8];

	/* "base" is the number of the first GPIO.  Dynamic assignment is
	 * not currently supported, and even if there are gaps in chip
	 * addressing the GPIO numbers are sequential .. so for example
	 * if only slaves 0 and 3 are present, their GPIOs range from
	 * base to base+15 (or base+31 for s17 variant).
	 */
	unsigned	base;
};
