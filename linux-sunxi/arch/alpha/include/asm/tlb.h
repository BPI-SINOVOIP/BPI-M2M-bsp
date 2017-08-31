/*
 * arch/alpha/include/asm/tlb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ALPHA_TLB_H
#define _ALPHA_TLB_H

#define tlb_start_vma(tlb, vma)			do { } while (0)
#define tlb_end_vma(tlb, vma)			do { } while (0)
#define __tlb_remove_tlb_entry(tlb, pte, addr)	do { } while (0)

#define tlb_flush(tlb)				flush_tlb_mm((tlb)->mm)

#include <asm-generic/tlb.h>

#define __pte_free_tlb(tlb, pte, address)		pte_free((tlb)->mm, pte)
#define __pmd_free_tlb(tlb, pmd, address)		pmd_free((tlb)->mm, pmd)
 
#endif
