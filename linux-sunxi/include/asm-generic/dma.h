/*
 * include/asm-generic/dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_DMA_H
#define __ASM_GENERIC_DMA_H
/*
 * This file traditionally describes the i8237 PC style DMA controller.
 * Most architectures don't have these any more and can get the minimal
 * implementation from kernel/dma.c by not defining MAX_DMA_CHANNELS.
 *
 * Some code relies on seeing MAX_DMA_ADDRESS though.
 */
#define MAX_DMA_ADDRESS PAGE_OFFSET

extern int request_dma(unsigned int dmanr, const char *device_id);
extern void free_dma(unsigned int dmanr);

#endif /* __ASM_GENERIC_DMA_H */
