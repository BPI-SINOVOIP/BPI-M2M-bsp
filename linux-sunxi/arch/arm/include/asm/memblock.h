/*
 * arch/arm/include/asm/memblock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_ARM_MEMBLOCK_H
#define _ASM_ARM_MEMBLOCK_H

struct meminfo;
struct machine_desc;

extern void arm_memblock_init(struct meminfo *, struct machine_desc *);

phys_addr_t arm_memblock_steal(phys_addr_t size, phys_addr_t align);

#endif
