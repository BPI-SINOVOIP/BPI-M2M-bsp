/*
 * arch/arm/mach-nomadik/cpu-8815.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
extern void cpu8815_map_io(void);
extern void cpu8815_platform_init(void);
extern void cpu8815_init_irq(void);
extern void cpu8815_restart(char, const char *);
