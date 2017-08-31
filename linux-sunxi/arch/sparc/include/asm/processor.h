/*
 * arch/sparc/include/asm/processor.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ___ASM_SPARC_PROCESSOR_H
#define ___ASM_SPARC_PROCESSOR_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/processor_64.h>
#else
#include <asm/processor_32.h>
#endif

#define nop() 		__asm__ __volatile__ ("nop")

#endif
