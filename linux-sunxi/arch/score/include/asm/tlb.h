/*
 * arch/score/include/asm/tlb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SCORE_TLB_H
#define _ASM_SCORE_TLB_H

/*
 * SCORE doesn't need any special per-pte or per-vma handling, except
 * we need to flush cache for area to be unmapped.
 */
#define tlb_start_vma(tlb, vma)		do {} while (0)
#define tlb_end_vma(tlb, vma)		do {} while (0)
#define __tlb_remove_tlb_entry(tlb, ptep, address) do {} while (0)
#define tlb_flush(tlb)			flush_tlb_mm((tlb)->mm)

extern void score7_FTLB_refill_Handler(void);

#include <asm-generic/tlb.h>

#endif /* _ASM_SCORE_TLB_H */
