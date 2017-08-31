/*
 * arch/arm/include/asm/current.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASMARM_CURRENT_H
#define _ASMARM_CURRENT_H

#include <linux/thread_info.h>

static inline struct task_struct *get_current(void) __attribute_const__;

static inline struct task_struct *get_current(void)
{
	return current_thread_info()->task;
}

#define current (get_current())

#endif /* _ASMARM_CURRENT_H */
