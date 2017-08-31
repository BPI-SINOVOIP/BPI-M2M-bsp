/*
 * arch/cris/include/arch-v10/arch/tlb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CRIS_ARCH_TLB_H
#define _CRIS_ARCH_TLB_H

/* The TLB can host up to 64 different mm contexts at the same time.
 * The last page_id is never running - it is used as an invalid page_id
 * so we can make TLB entries that will never match. 
 */
#define NUM_TLB_ENTRIES 64
#define NUM_PAGEID 64
#define INVALID_PAGEID 63
#define NO_CONTEXT -1

#endif
