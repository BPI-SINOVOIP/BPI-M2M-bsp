/*
 * arch/alpha/include/asm/mmu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ALPHA_MMU_H
#define __ALPHA_MMU_H

/* The alpha MMU context is one "unsigned long" bitmap per CPU */
typedef unsigned long mm_context_t[NR_CPUS];

#endif
