/*
 * arch/arm/mach-clps711x/common.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * linux/arch/arm/mach-clps711x/common.h
 *
 * Common bits.
 */

struct sys_timer;

extern void clps711x_map_io(void);
extern void clps711x_init_irq(void);
extern struct sys_timer clps711x_timer;
extern void clps711x_restart(char mode, const char *cmd);
