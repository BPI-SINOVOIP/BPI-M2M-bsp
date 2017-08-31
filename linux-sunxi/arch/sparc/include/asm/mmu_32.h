/*
 * arch/sparc/include/asm/mmu_32.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MMU_H
#define __MMU_H

/* Default "unsigned long" context */
typedef unsigned long mm_context_t;

/* mm/srmmu.c */
extern ctxd_t *srmmu_ctx_table_phys;

#endif
