/*
 * arch/sparc/include/asm/bitops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ___ASM_SPARC_BITOPS_H
#define ___ASM_SPARC_BITOPS_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/bitops_64.h>
#else
#include <asm/bitops_32.h>
#endif
#endif
