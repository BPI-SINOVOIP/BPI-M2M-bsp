/*
 * arch/m68k/include/asm/swab.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _M68K_SWAB_H
#define _M68K_SWAB_H

#include <linux/types.h>
#include <linux/compiler.h>

#define __SWAB_64_THRU_32__

#if defined (__mcfisaaplus__) || defined (__mcfisac__)
static inline __attribute_const__ __u32 __arch_swab32(__u32 val)
{
	__asm__("byterev %0" : "=d" (val) : "0" (val));
	return val;
}

#define __arch_swab32 __arch_swab32
#elif !defined(__mcoldfire__)

static inline __attribute_const__ __u32 __arch_swab32(__u32 val)
{
	__asm__("rolw #8,%0; swap %0; rolw #8,%0" : "=d" (val) : "0" (val));
	return val;
}
#define __arch_swab32 __arch_swab32
#endif

#endif /* _M68K_SWAB_H */
