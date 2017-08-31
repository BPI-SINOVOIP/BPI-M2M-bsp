/*
 * arch/arm/plat-versatile/include/plat/sched_clock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ARM_PLAT_SCHED_CLOCK_H
#define ARM_PLAT_SCHED_CLOCK_H

void versatile_sched_clock_init(void __iomem *, unsigned long);

#endif
