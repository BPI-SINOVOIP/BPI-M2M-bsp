/*
 * arch/powerpc/math-emu/fnabs.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

int
fnabs(u32 *frD, u32 *frB)
{
	frD[0] = frB[0] | 0x80000000;
	frD[1] = frB[1];

#ifdef DEBUG
	printk("%s: D %p, B %p: ", __func__, frD, frB);
	dump_double(frD);
	printk("\n");
#endif

	return 0;
}
