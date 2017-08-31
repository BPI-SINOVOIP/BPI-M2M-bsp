/*
 * arch/score/include/asm/unistd.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#if !defined(_ASM_SCORE_UNISTD_H) || defined(__SYSCALL)
#define _ASM_SCORE_UNISTD_H

#define __ARCH_HAVE_MMU

#define __ARCH_WANT_SYSCALL_NO_AT
#define __ARCH_WANT_SYSCALL_NO_FLAGS
#define __ARCH_WANT_SYSCALL_OFF_T
#define __ARCH_WANT_SYSCALL_DEPRECATED

#include <asm-generic/unistd.h>

#endif /* _ASM_SCORE_UNISTD_H */
