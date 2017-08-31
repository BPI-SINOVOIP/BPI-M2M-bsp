/*
 * arch/x86/include/asm/time.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_TIME_H
#define _ASM_X86_TIME_H

#include <linux/clocksource.h>
#include <asm/mc146818rtc.h>

extern void hpet_time_init(void);
extern void time_init(void);

extern struct clock_event_device *global_clock_event;

#endif /* _ASM_X86_TIME_H */
