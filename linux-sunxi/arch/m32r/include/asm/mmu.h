/*
 * arch/m32r/include/asm/mmu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_M32R_MMU_H
#define _ASM_M32R_MMU_H

#if !defined(CONFIG_MMU)

typedef struct {
	unsigned long		end_brk;
} mm_context_t;

#else /* CONFIG_MMU */

/* Default "unsigned long" context */
#ifndef CONFIG_SMP
typedef unsigned long mm_context_t;
#else
typedef unsigned long mm_context_t[NR_CPUS];
#endif

#endif /* CONFIG_MMU */

#endif /* _ASM_M32R_MMU_H */
