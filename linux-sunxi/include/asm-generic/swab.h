/*
 * include/asm-generic/swab.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_GENERIC_SWAB_H
#define _ASM_GENERIC_SWAB_H

#include <asm/bitsperlong.h>

/*
 * 32 bit architectures typically (but not always) want to
 * set __SWAB_64_THRU_32__. In user space, this is only
 * valid if the compiler supports 64 bit data types.
 */

#if __BITS_PER_LONG == 32
#if defined(__GNUC__) && !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#define __SWAB_64_THRU_32__
#endif
#endif

#endif /* _ASM_GENERIC_SWAB_H */
