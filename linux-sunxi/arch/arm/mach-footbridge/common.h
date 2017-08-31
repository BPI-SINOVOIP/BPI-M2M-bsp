/*
 * arch/arm/mach-footbridge/common.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

extern struct sys_timer footbridge_timer;
extern struct sys_timer isa_timer;

extern void isa_rtc_init(void);

extern void footbridge_map_io(void);
extern void footbridge_init_irq(void);

extern void isa_init_irq(unsigned int irq);
extern void footbridge_restart(char, const char *);
