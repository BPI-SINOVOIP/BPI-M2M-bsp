/*
 * arch/arm/mach-integrator/common.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
void integrator_init_early(void);
void integrator_reserve(void);
void integrator_restart(char, const char *);
