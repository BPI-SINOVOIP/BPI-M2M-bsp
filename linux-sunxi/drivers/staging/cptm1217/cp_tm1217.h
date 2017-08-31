/*
 * drivers/staging/cptm1217/cp_tm1217.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_I2C_CP_TM1217_H
#define __LINUX_I2C_CP_TM1217_H

struct cp_tm1217_platform_data
{
	int gpio;		/* If not set uses the IRQ resource 0 */
};

#endif
