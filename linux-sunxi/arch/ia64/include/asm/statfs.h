/*
 * arch/ia64/include/asm/statfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_STATFS_H
#define _ASM_IA64_STATFS_H

/*
 * Based on <asm-i386/statfs.h>.
 *
 * Modified 1998, 1999, 2003
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co
 */

/*
 * We need compat_statfs64 to be packed, because the i386 ABI won't
 * add padding at the end to bring it to a multiple of 8 bytes, but
 * the IA64 ABI will.
 */
#define ARCH_PACK_COMPAT_STATFS64 __attribute__((packed,aligned(4)))

#include <asm-generic/statfs.h>

#endif /* _ASM_IA64_STATFS_H */
