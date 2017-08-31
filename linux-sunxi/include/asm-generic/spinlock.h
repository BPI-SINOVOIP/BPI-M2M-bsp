/*
 * include/asm-generic/spinlock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_SPINLOCK_H
#define __ASM_GENERIC_SPINLOCK_H
/*
 * You need to implement asm/spinlock.h for SMP support. The generic
 * version does not handle SMP.
 */
#ifdef CONFIG_SMP
#error need an architecture specific asm/spinlock.h
#endif

#endif /* __ASM_GENERIC_SPINLOCK_H */
