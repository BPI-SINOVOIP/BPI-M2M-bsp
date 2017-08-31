/*
 * arch/mips/include/asm/mach-bcm63xx/bcm63xx_clk.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef BCM63XX_CLK_H_
#define BCM63XX_CLK_H_

struct clk {
	void		(*set)(struct clk *, int);
	unsigned int	rate;
	unsigned int	usage;
	int		id;
};

#endif /* ! BCM63XX_CLK_H_ */
