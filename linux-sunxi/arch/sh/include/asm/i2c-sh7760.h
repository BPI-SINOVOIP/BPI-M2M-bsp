/*
 * arch/sh/include/asm/i2c-sh7760.h
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
 * MMIO/IRQ and platform data for SH7760 I2C channels
 */

#ifndef _I2C_SH7760_H_
#define _I2C_SH7760_H_

#define SH7760_I2C_DEVNAME	"sh7760-i2c"

#define SH7760_I2C0_MMIO	0xFE140000
#define SH7760_I2C0_MMIOEND	0xFE14003B
#define SH7760_I2C0_IRQ		62

#define SH7760_I2C1_MMIO	0xFE150000
#define SH7760_I2C1_MMIOEND	0xFE15003B
#define SH7760_I2C1_IRQ		63

struct sh7760_i2c_platdata {
	unsigned int speed_khz;
};

#endif
