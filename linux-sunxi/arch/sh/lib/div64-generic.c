/*
 * arch/sh/lib/div64-generic.c
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
 * Generic __div64_32 wrapper for __xdiv64_32.
 */

#include <linux/types.h>
#include <asm/div64.h>

extern uint64_t __xdiv64_32(u64 n, u32 d);

uint32_t __div64_32(u64 *xp, u32 y)
{
	uint32_t rem;
	uint64_t q = __xdiv64_32(*xp, y);

	rem = *xp - q * y;
	*xp = q;

	return rem;
}
