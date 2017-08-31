/*
 * arch/sparc/include/asm/irqflags.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ___ASM_SPARC_IRQFLAGS_H
#define ___ASM_SPARC_IRQFLAGS_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/irqflags_64.h>
#else
#include <asm/irqflags_32.h>
#endif
#endif
