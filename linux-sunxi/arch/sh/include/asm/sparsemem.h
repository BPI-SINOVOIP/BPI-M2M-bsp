/*
 * arch/sh/include/asm/sparsemem.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_SPARSEMEM_H
#define __ASM_SH_SPARSEMEM_H

#ifdef __KERNEL__
/*
 * SECTION_SIZE_BITS		2^N: how big each section will be
 * MAX_PHYSADDR_BITS		2^N: how much physical address space we have
 * MAX_PHYSMEM_BITS		2^N: how much memory we can have in that space
 */
#define SECTION_SIZE_BITS	26
#define MAX_PHYSADDR_BITS	32
#define MAX_PHYSMEM_BITS	32

#endif

#endif /* __ASM_SH_SPARSEMEM_H */
