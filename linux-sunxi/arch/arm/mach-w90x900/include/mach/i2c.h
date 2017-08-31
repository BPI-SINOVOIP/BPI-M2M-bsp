/*
 * arch/arm/mach-w90x900/include/mach/i2c.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARCH_NUC900_I2C_H
#define __ASM_ARCH_NUC900_I2C_H

struct nuc900_platform_i2c {
	int		bus_num;
	unsigned long   bus_freq;
};

#endif /* __ASM_ARCH_NUC900_I2C_H */
