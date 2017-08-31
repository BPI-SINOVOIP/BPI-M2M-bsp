/*
 * include/linux/i2c/max6639.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_MAX6639_H
#define _LINUX_MAX6639_H

#include <linux/types.h>

/* platform data for the MAX6639 temperature sensor and fan control */

struct max6639_platform_data {
	bool pwm_polarity;	/* Polarity low (0) or high (1, default) */
	int ppr;		/* Pulses per rotation 1..4 (default == 2) */
	int rpm_range;		/* 2000, 4000 (default), 8000 or 16000 */
};

#endif /* _LINUX_MAX6639_H */
