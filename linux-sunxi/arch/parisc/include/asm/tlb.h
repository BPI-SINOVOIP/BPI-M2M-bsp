/*
 * arch/parisc/include/asm/tlb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PARISC_TLB_H
#define _PARISC_TLB_H

#define tlb_flush(tlb)			\
do {	if ((tlb)->fullmm)		\
		flush_tlb_mm((tlb)->mm);\
} while (0)

#define tlb_start_vma(tlb, vma) \
do {	if (!(tlb)->fullmm)	\
		flush_cache_range(vma, vma->vm_start, vma->vm_end); \
} while (0)

#define tlb_end_vma(tlb, vma)	\
do {	if (!(tlb)->fullmm)	\
		flush_tlb_range(vma, vma->vm_start, vma->vm_end); \
} while (0)

#define __tlb_remove_tlb_entry(tlb, pte, address) \
	do { } while (0)

#include <asm-generic/tlb.h>

#define __pmd_free_tlb(tlb, pmd, addr)	pmd_free((tlb)->mm, pmd)
#define __pte_free_tlb(tlb, pte, addr)	pte_free((tlb)->mm, pte)

#endif
