/*
 * arch/sparc/include/asm/iommu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ___ASM_SPARC_IOMMU_H
#define ___ASM_SPARC_IOMMU_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/iommu_64.h>
#else
#include <asm/iommu_32.h>
#endif
#endif
