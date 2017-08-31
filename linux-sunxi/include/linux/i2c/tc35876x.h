/*
 * include/linux/i2c/tc35876x.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef _TC35876X_H
#define _TC35876X_H

struct tc35876x_platform_data {
	int gpio_bridge_reset;
	int gpio_panel_bl_en;
	int gpio_panel_vadd;
};

#endif /* _TC35876X_H */
