/*
 * arch/ia64/include/asm/sparsemem.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_SPARSEMEM_H
#define _ASM_IA64_SPARSEMEM_H

#ifdef CONFIG_SPARSEMEM
/*
 * SECTION_SIZE_BITS            2^N: how big each section will be
 * MAX_PHYSMEM_BITS             2^N: how much memory we can have in that space
 */

#define SECTION_SIZE_BITS	(30)
#define MAX_PHYSMEM_BITS	(50)
#ifdef CONFIG_FORCE_MAX_ZONEORDER
#if ((CONFIG_FORCE_MAX_ZONEORDER - 1 + PAGE_SHIFT) > SECTION_SIZE_BITS)
#undef SECTION_SIZE_BITS
#define SECTION_SIZE_BITS (CONFIG_FORCE_MAX_ZONEORDER - 1 + PAGE_SHIFT)
#endif
#endif

#endif /* CONFIG_SPARSEMEM */
#endif /* _ASM_IA64_SPARSEMEM_H */
