/*
 * include/linux/input/ili210x.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ILI210X_H
#define _ILI210X_H

struct ili210x_platform_data {
	unsigned long irq_flags;
	unsigned int poll_period;
	bool (*get_pendown_state)(void);
};

#endif
