/*
 * include/linux/input/eeti_ts.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_INPUT_EETI_TS_H
#define LINUX_INPUT_EETI_TS_H

struct eeti_ts_platform_data {
	int irq_gpio;
	unsigned int irq_active_high;
};

#endif /* LINUX_INPUT_EETI_TS_H */

