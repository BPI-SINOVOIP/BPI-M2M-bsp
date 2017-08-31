/*
 * include/asm-generic/mmu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_MMU_H
#define __ASM_GENERIC_MMU_H

/*
 * This is the mmu.h header for nommu implementations.
 * Architectures with an MMU need something more complex.
 */
#ifndef __ASSEMBLY__
typedef struct {
	struct vm_list_struct	*vmlist;
	unsigned long		end_brk;
} mm_context_t;
#endif

#endif /* __ASM_GENERIC_MMU_H */
