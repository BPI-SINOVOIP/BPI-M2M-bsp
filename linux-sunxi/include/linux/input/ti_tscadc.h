/*
 * include/linux/input/ti_tscadc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_TI_TSCADC_H
#define __LINUX_TI_TSCADC_H

/**
 * struct tsc_data	Touchscreen wire configuration
 * @wires:		Wires refer to application modes
 *			i.e. 4/5/8 wire touchscreen support
 *			on the platform.
 * @x_plate_resistance:	X plate resistance.
 */

struct tsc_data {
	int wires;
	int x_plate_resistance;
};

#endif
