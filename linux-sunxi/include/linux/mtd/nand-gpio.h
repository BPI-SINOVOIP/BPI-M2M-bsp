/*
 * include/linux/mtd/nand-gpio.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_MTD_NAND_GPIO_H
#define __LINUX_MTD_NAND_GPIO_H

#include <linux/mtd/nand.h>

struct gpio_nand_platdata {
	int	gpio_nce;
	int	gpio_nwp;
	int	gpio_cle;
	int	gpio_ale;
	int	gpio_rdy;
	void	(*adjust_parts)(struct gpio_nand_platdata *, size_t);
	struct mtd_partition *parts;
	unsigned int num_parts;
	unsigned int options;
	int	chip_delay;
};

#endif
