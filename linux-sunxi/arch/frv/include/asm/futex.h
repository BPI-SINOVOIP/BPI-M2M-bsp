/*
 * arch/frv/include/asm/futex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_FUTEX_H
#define _ASM_FUTEX_H

#ifdef __KERNEL__

#include <linux/futex.h>
#include <asm/errno.h>
#include <asm/uaccess.h>

extern int futex_atomic_op_inuser(int encoded_op, u32 __user *uaddr);

static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
	return -ENOSYS;
}

#endif
#endif
