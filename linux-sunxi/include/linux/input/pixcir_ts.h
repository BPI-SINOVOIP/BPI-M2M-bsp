/*
 * include/linux/input/pixcir_ts.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef	_PIXCIR_I2C_TS_H
#define	_PIXCIR_I2C_TS_H

struct pixcir_ts_platform_data {
	int (*attb_read_val)(void);
	int x_max;
	int y_max;
};

#endif
