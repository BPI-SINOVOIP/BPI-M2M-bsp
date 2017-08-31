/*
 * arch/avr32/include/asm/mmu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_MMU_H
#define __ASM_AVR32_MMU_H

/* Default "unsigned long" context */
typedef unsigned long mm_context_t;

#define MMU_ITLB_ENTRIES	64
#define MMU_DTLB_ENTRIES	64

#endif /* __ASM_AVR32_MMU_H */
