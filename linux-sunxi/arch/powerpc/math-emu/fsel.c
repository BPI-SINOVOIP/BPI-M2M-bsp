/*
 * arch/powerpc/math-emu/fsel.c
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

#include <asm/sfp-machine.h>
#include <math-emu/soft-fp.h>
#include <math-emu/double.h>

int
fsel(u32 *frD, void *frA, u32 *frB, u32 *frC)
{
	FP_DECL_D(A);
	FP_DECL_EX;

#ifdef DEBUG
	printk("%s: %p %p %p %p\n", __func__, frD, frA, frB, frC);
#endif

	FP_UNPACK_DP(A, frA);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
	printk("B: %08x %08x\n", frB[0], frB[1]);
	printk("C: %08x %08x\n", frC[0], frC[1]);
#endif

	if (A_c == FP_CLS_NAN || (A_c != FP_CLS_ZERO && A_s)) {
		frD[0] = frB[0];
		frD[1] = frB[1];
	} else {
		frD[0] = frC[0];
		frD[1] = frC[1];
	}

#ifdef DEBUG
	printk("D: %08x.%08x\n", frD[0], frD[1]);
#endif

	return 0;
}
