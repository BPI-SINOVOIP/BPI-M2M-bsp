/*
 * arch/frv/include/asm/timex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* timex.h: FR-V architecture timex specifications
 */
#ifndef _ASM_TIMEX_H
#define _ASM_TIMEX_H

#define CLOCK_TICK_RATE		1193180 /* Underlying HZ */
#define CLOCK_TICK_FACTOR	20	/* Factor of both 1000000 and CLOCK_TICK_RATE */

typedef unsigned long cycles_t;

static inline cycles_t get_cycles(void)
{
	return 0;
}

#define vxtime_lock()		do {} while (0)
#define vxtime_unlock()		do {} while (0)

#endif

