/*
 * arch/powerpc/math-emu/lfd.c
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
#include <math-emu/double.h>

int
lfd(void *frD, void *ea)
{
	if (copy_from_user(frD, ea, sizeof(double)))
		return -EFAULT;
#ifdef DEBUG
	printk("%s: D %p, ea %p: ", __func__, frD, ea);
	dump_double(frD);
	printk("\n");
#endif
	return 0;
}
