/*
 * arch/ia64/include/asm/posix_types.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_POSIX_TYPES_H
#define _ASM_IA64_POSIX_TYPES_H

typedef unsigned int	__kernel_nlink_t;
#define __kernel_nlink_t __kernel_nlink_t

typedef unsigned long	__kernel_sigset_t;	/* at least 32 bits */

#include <asm-generic/posix_types.h>

#endif /* _ASM_IA64_POSIX_TYPES_H */
