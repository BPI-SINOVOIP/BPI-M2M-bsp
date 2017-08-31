/*
 * arch/s390/include/asm/sparsemem.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_S390_SPARSEMEM_H
#define _ASM_S390_SPARSEMEM_H

#ifdef CONFIG_64BIT

#define SECTION_SIZE_BITS	28
#define MAX_PHYSADDR_BITS	46
#define MAX_PHYSMEM_BITS	46

#else

#define SECTION_SIZE_BITS	25
#define MAX_PHYSADDR_BITS	31
#define MAX_PHYSMEM_BITS	31

#endif /* CONFIG_64BIT */

#endif /* _ASM_S390_SPARSEMEM_H */
