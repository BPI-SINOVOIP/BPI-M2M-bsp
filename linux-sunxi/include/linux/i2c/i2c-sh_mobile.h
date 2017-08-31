/*
 * include/linux/i2c/i2c-sh_mobile.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __I2C_SH_MOBILE_H__
#define __I2C_SH_MOBILE_H__

#include <linux/platform_device.h>

struct i2c_sh_mobile_platform_data {
	unsigned long bus_speed;
};

#endif /* __I2C_SH_MOBILE_H__ */
