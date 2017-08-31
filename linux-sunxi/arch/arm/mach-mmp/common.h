/*
 * arch/arm/mach-mmp/common.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

struct sys_timer;

extern void timer_init(int irq);

extern void __init icu_init_irq(void);
extern void __init mmp_map_io(void);
extern void mmp_restart(char, const char *);
