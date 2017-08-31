/*
 * arch/arm/include/asm/scatterlist.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASMARM_SCATTERLIST_H
#define _ASMARM_SCATTERLIST_H

#ifdef CONFIG_ARM_HAS_SG_CHAIN
#define ARCH_HAS_SG_CHAIN
#endif

#include <asm/memory.h>
#include <asm/types.h>
#include <asm-generic/scatterlist.h>

#endif /* _ASMARM_SCATTERLIST_H */
