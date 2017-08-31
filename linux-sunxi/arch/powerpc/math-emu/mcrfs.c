/*
 * arch/powerpc/math-emu/mcrfs.c
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

int
mcrfs(u32 *ccr, u32 crfD, u32 crfS)
{
	u32 value, clear;

#ifdef DEBUG
	printk("%s: %p (%08x) %d %d\n", __func__, ccr, *ccr, crfD, crfS);
#endif

	clear = 15 << ((7 - crfS) << 2);
	if (!crfS)
		clear = 0x90000000;

	value = (__FPU_FPSCR >> ((7 - crfS) << 2)) & 15;
	__FPU_FPSCR &= ~(clear);

	*ccr &= ~(15 << ((7 - crfD) << 2));
	*ccr |= (value << ((7 - crfD) << 2));

#ifdef DEBUG
	printk("CR: %08x\n", __func__, *ccr);
#endif

	return 0;
}
