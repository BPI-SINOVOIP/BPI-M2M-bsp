/*
 * include/linux/amba/pl061.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/types.h>

/* platform data for the PL061 GPIO driver */

struct pl061_platform_data {
	/* number of the first GPIO */
	unsigned	gpio_base;

	/* number of the first IRQ.
	 * If the IRQ functionality in not desired this must be set to 0.
	 */
	unsigned	irq_base;

	u8		directions;	/* startup directions, 1: out, 0: in */
	u8		values;		/* startup values */
};
