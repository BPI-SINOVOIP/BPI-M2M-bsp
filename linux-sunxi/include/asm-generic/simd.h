/*
 * include/asm-generic/simd.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/hardirq.h>

/*
 * may_use_simd - whether it is allowable at this time to issue SIMD
 *                instructions or access the SIMD register file
 *
 * As architectures typically don't preserve the SIMD register file when
 * taking an interrupt, !in_interrupt() should be a reasonable default.
 */
static __must_check inline bool may_use_simd(void)
{
	return !in_interrupt();
}
