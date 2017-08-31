/*
 * arch/parisc/include/asm/bitsperlong.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_PARISC_BITSPERLONG_H
#define __ASM_PARISC_BITSPERLONG_H

/*
 * using CONFIG_* outside of __KERNEL__ is wrong,
 * __LP64__ was also removed from headers, so what
 * is the right approach on parisc?
 *	-arnd
 */
#if (defined(__KERNEL__) && defined(CONFIG_64BIT)) || defined (__LP64__)
#define __BITS_PER_LONG 64
#define SHIFT_PER_LONG 6
#else
#define __BITS_PER_LONG 32
#define SHIFT_PER_LONG 5
#endif

#include <asm-generic/bitsperlong.h>

#endif /* __ASM_PARISC_BITSPERLONG_H */
