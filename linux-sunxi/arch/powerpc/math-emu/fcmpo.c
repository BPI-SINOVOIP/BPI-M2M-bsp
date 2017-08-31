/*
 * arch/powerpc/math-emu/fcmpo.c
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
fcmpo(u32 *ccr, int crfD, void *frA, void *frB)
{
	FP_DECL_D(A);
	FP_DECL_D(B);
	FP_DECL_EX;
	int code[4] = { (1 << 3), (1 << 1), (1 << 2), (1 << 0) };
	long cmp;

#ifdef DEBUG
	printk("%s: %p (%08x) %d %p %p\n", __func__, ccr, *ccr, crfD, frA, frB);
#endif

	FP_UNPACK_DP(A, frA);
	FP_UNPACK_DP(B, frB);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
	printk("B: %ld %lu %lu %ld (%ld)\n", B_s, B_f1, B_f0, B_e, B_c);
#endif

	if (A_c == FP_CLS_NAN || B_c == FP_CLS_NAN)
		FP_SET_EXCEPTION(EFLAG_VXVC);

	FP_CMP_D(cmp, A, B, 2);
	cmp = code[(cmp + 1) & 3];

	__FPU_FPSCR &= ~(0x1f000);
	__FPU_FPSCR |= (cmp << 12);

	*ccr &= ~(15 << ((7 - crfD) << 2));
	*ccr |= (cmp << ((7 - crfD) << 2));

#ifdef DEBUG
	printk("CR: %08x\n", *ccr);
#endif

	return FP_CUR_EXCEPTIONS;
}
