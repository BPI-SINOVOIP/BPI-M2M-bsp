/*
 * arch/powerpc/include/asm/errno.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_POWERPC_ERRNO_H
#define _ASM_POWERPC_ERRNO_H

#include <asm-generic/errno.h>

#undef	EDEADLOCK
#define	EDEADLOCK	58	/* File locking deadlock error */

#define _LAST_ERRNO	516

#endif	/* _ASM_POWERPC_ERRNO_H */
