/*
 * arch/mips/include/asm/mach-ip32/kmalloc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MACH_IP32_KMALLOC_H
#define __ASM_MACH_IP32_KMALLOC_H


#if defined(CONFIG_CPU_R5000) || defined(CONFIG_CPU_RM7000)
#define ARCH_DMA_MINALIGN	32
#else
#define ARCH_DMA_MINALIGN	128
#endif

#endif /* __ASM_MACH_IP32_KMALLOC_H */
