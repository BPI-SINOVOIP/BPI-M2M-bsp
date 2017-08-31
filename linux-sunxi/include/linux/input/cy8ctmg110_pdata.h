/*
 * include/linux/input/cy8ctmg110_pdata.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_CY8CTMG110_PDATA_H
#define _LINUX_CY8CTMG110_PDATA_H

struct cy8ctmg110_pdata
{
	int reset_pin;		/* Reset pin is wired to this GPIO (optional) */
	int irq_pin;		/* IRQ pin is wired to this GPIO */
};

#endif
