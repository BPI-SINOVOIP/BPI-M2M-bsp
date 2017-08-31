/*
 * arch/cris/include/asm/tlb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CRIS_TLB_H
#define _CRIS_TLB_H

#include <linux/pagemap.h>

#include <arch/tlb.h>

/*
 * cris doesn't need any special per-pte or
 * per-vma handling..
 */
#define tlb_start_vma(tlb, vma) do { } while (0)
#define tlb_end_vma(tlb, vma) do { } while (0)
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)

#define tlb_flush(tlb) flush_tlb_mm((tlb)->mm)
#include <asm-generic/tlb.h>

#endif
