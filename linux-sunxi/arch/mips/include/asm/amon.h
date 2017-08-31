/*
 * arch/mips/include/asm/amon.h
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
 * Amon support
 */

int amon_cpu_avail(int);
void amon_cpu_start(int, unsigned long, unsigned long,
		    unsigned long, unsigned long);
