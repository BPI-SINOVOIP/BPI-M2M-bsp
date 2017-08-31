/*
 * arch/m32r/include/asm/timex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_M32R_TIMEX_H
#define _ASM_M32R_TIMEX_H

/*
 * linux/include/asm-m32r/timex.h
 *
 * m32r architecture timex specifications
 */

#define CLOCK_TICK_RATE	(CONFIG_BUS_CLOCK / CONFIG_TIMER_DIVIDE)
#define CLOCK_TICK_FACTOR	20	/* Factor of both 1000000 and CLOCK_TICK_RATE */

#ifdef __KERNEL__
/*
 * Standard way to access the cycle counter.
 * Currently only used on SMP.
 */

typedef unsigned long long cycles_t;

static __inline__ cycles_t get_cycles (void)
{
	return 0;
}
#endif  /* __KERNEL__ */

#endif  /* _ASM_M32R_TIMEX_H */
