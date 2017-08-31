/*
 * include/linux/i2c/s6000.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_I2C_S6000_H
#define __LINUX_I2C_S6000_H

struct s6_i2c_platform_data {
	const char *clock; /* the clock to use */
	int bus_num; /* the bus number to register */
};

#endif

