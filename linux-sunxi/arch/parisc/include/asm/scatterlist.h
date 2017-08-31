/*
 * arch/parisc/include/asm/scatterlist.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_PARISC_SCATTERLIST_H
#define _ASM_PARISC_SCATTERLIST_H

#include <asm/page.h>
#include <asm/types.h>
#include <asm-generic/scatterlist.h>

#define sg_virt_addr(sg) ((unsigned long)sg_virt(sg))

#endif /* _ASM_PARISC_SCATTERLIST_H */
