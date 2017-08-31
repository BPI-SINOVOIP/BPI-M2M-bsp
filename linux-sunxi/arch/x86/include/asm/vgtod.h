/*
 * arch/x86/include/asm/vgtod.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_VGTOD_H
#define _ASM_X86_VGTOD_H

#include <asm/vsyscall.h>
#include <linux/clocksource.h>

struct vsyscall_gtod_data {
	seqcount_t	seq;

	struct { /* extract of a clocksource struct */
		int vclock_mode;
		cycle_t	cycle_last;
		cycle_t	mask;
		u32	mult;
		u32	shift;
	} clock;

	/* open coded 'struct timespec' */
	time_t		wall_time_sec;
	u32		wall_time_nsec;
	u32		monotonic_time_nsec;
	time_t		monotonic_time_sec;

	struct timezone sys_tz;
	struct timespec wall_time_coarse;
	struct timespec monotonic_time_coarse;
};
extern struct vsyscall_gtod_data vsyscall_gtod_data;

#endif /* _ASM_X86_VGTOD_H */
