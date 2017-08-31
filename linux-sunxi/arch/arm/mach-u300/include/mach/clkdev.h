/*
 * arch/arm/mach-u300/include/mach/clkdev.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MACH_CLKDEV_H
#define __MACH_CLKDEV_H

int __clk_get(struct clk *clk);
void __clk_put(struct clk *clk);

#endif
