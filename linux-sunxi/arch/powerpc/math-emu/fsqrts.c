/*
 * arch/powerpc/math-emu/fsqrts.c
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
#include <math-emu/single.h>

int
fsqrts(void *frD, void *frB)
{
	FP_DECL_D(B);
	FP_DECL_D(R);
	FP_DECL_EX;

#ifdef DEBUG
	printk("%s: %p %p %p %p\n", __func__, frD, frB);
#endif

	FP_UNPACK_DP(B, frB);

#ifdef DEBUG
	printk("B: %ld %lu %lu %ld (%ld)\n", B_s, B_f1, B_f0, B_e, B_c);
#endif

	if (B_s && B_c != FP_CLS_ZERO)
		FP_SET_EXCEPTION(EFLAG_VXSQRT);
	if (B_c == FP_CLS_NAN)
		FP_SET_EXCEPTION(EFLAG_VXSNAN);

	FP_SQRT_D(R, B);

#ifdef DEBUG
	printk("R: %ld %lu %lu %ld (%ld)\n", R_s, R_f1, R_f0, R_e, R_c);
#endif

	__FP_PACK_DS(frD, R);

	return FP_CUR_EXCEPTIONS;
}
