/*
 * arch/sh/include/asm/bl_bit_32.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_BL_BIT_32_H
#define __ASM_SH_BL_BIT_32_H

static inline void set_bl_bit(void)
{
	unsigned long __dummy0, __dummy1;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"or	%2, %0\n\t"
		"and	%3, %0\n\t"
		"ldc	%0, sr\n\t"
		: "=&r" (__dummy0), "=r" (__dummy1)
		: "r" (0x10000000), "r" (0xffffff0f)
		: "memory"
	);
}

static inline void clear_bl_bit(void)
{
	unsigned long __dummy0, __dummy1;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"and	%2, %0\n\t"
		"ldc	%0, sr\n\t"
		: "=&r" (__dummy0), "=r" (__dummy1)
		: "1" (~0x10000000)
		: "memory"
	);
}

#endif /* __ASM_SH_BL_BIT_32_H */
