/*
 * arch/microblaze/include/asm/atomic.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_MICROBLAZE_ATOMIC_H
#define _ASM_MICROBLAZE_ATOMIC_H

#include <asm/cmpxchg.h>
#include <asm-generic/atomic.h>
#include <asm-generic/atomic64.h>

/*
 * Atomically test *v and decrement if it is greater than 0.
 * The function returns the old value of *v minus 1.
 */
static inline int atomic_dec_if_positive(atomic_t *v)
{
	unsigned long flags;
	int res;

	local_irq_save(flags);
	res = v->counter - 1;
	if (res >= 0)
		v->counter = res;
	local_irq_restore(flags);

	return res;
}

#endif /* _ASM_MICROBLAZE_ATOMIC_H */
