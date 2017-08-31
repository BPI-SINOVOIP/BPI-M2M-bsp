/*
 * arch/ia64/include/asm/clocksource.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* IA64-specific clocksource additions */

#ifndef _ASM_IA64_CLOCKSOURCE_H
#define _ASM_IA64_CLOCKSOURCE_H

struct arch_clocksource_data {
	void *fsys_mmio;        /* used by fsyscall asm code */
};

#endif /* _ASM_IA64_CLOCKSOURCE_H */
