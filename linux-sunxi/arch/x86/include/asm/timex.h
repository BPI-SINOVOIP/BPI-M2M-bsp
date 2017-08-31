/*
 * arch/x86/include/asm/timex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_TIMEX_H
#define _ASM_X86_TIMEX_H

#include <asm/processor.h>
#include <asm/tsc.h>

/* Assume we use the PIT time source for the clock tick */
#define CLOCK_TICK_RATE		PIT_TICK_RATE

#define ARCH_HAS_READ_CURRENT_TIMER

#endif /* _ASM_X86_TIMEX_H */
