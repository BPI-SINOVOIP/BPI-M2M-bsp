/*
 * arch/arm/include/asm/arch_timer.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASMARM_ARCH_TIMER_H
#define __ASMARM_ARCH_TIMER_H

#include <asm/errno.h>
#include <linux/ioport.h>

struct arch_timer {
	struct resource	res[2];
};

#ifdef CONFIG_ARM_ARCH_TIMER
int arch_timer_register(struct arch_timer *);
int arch_timer_of_register(void);
int arch_timer_sched_clock_init(void);
#else
static inline int arch_timer_register(struct arch_timer *at)
{
	return -ENXIO;
}

static inline int arch_timer_of_register(void)
{
	return -ENXIO;
}

static inline int arch_timer_sched_clock_init(void)
{
	return -ENXIO;
}
#endif

#endif
