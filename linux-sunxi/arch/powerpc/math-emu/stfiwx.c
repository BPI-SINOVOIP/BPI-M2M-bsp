/*
 * arch/powerpc/math-emu/stfiwx.c
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
stfiwx(u32 *frS, void *ea)
{
#ifdef DEBUG
	printk("%s: %p %p\n", __func__, frS, ea);
#endif

	if (copy_to_user(ea, &frS[1], sizeof(frS[1])))
		return -EFAULT;

	return 0;
}
