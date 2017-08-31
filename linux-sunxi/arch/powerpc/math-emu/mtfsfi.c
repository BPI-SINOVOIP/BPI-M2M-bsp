/*
 * arch/powerpc/math-emu/mtfsfi.c
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
mtfsfi(unsigned int crfD, unsigned int IMM)
{
	u32 mask = 0xf;

	if (!crfD)
		mask = 9;

	__FPU_FPSCR &= ~(mask << ((7 - crfD) << 2));
	__FPU_FPSCR |= (IMM & 0xf) << ((7 - crfD) << 2);

#ifdef DEBUG
	printk("%s: %d %x: %08lx\n", __func__, crfD, IMM, __FPU_FPSCR);
#endif

	return 0;
}
