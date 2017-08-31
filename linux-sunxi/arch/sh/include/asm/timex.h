/*
 * arch/sh/include/asm/timex.h
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
 * linux/include/asm-sh/timex.h
 *
 * sh architecture timex specifications
 */
#ifndef __ASM_SH_TIMEX_H
#define __ASM_SH_TIMEX_H

/*
 * Only parts using the legacy CPG code for their clock framework
 * implementation need to define their own Pclk value. If provided, this
 * can be used for accurately setting CLOCK_TICK_RATE, otherwise we
 * simply fall back on the i8253 PIT value.
 */
#ifdef CONFIG_SH_PCLK_FREQ
#define CLOCK_TICK_RATE		(CONFIG_SH_PCLK_FREQ / 4) /* Underlying HZ */
#else
#define CLOCK_TICK_RATE		1193180
#endif

#include <asm-generic/timex.h>

#endif /* __ASM_SH_TIMEX_H */
