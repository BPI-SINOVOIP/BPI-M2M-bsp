/*
 * arch/parisc/include/asm/spinlock_types.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SPINLOCK_TYPES_H
#define __ASM_SPINLOCK_TYPES_H

typedef struct {
#ifdef CONFIG_PA20
	volatile unsigned int slock;
# define __ARCH_SPIN_LOCK_UNLOCKED { 1 }
#else
	volatile unsigned int lock[4];
# define __ARCH_SPIN_LOCK_UNLOCKED	{ { 1, 1, 1, 1 } }
#endif
} arch_spinlock_t;

typedef struct {
	arch_spinlock_t lock;
	volatile int counter;
} arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		{ __ARCH_SPIN_LOCK_UNLOCKED, 0 }

#endif
