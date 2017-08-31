/*
 * arch/s390/include/asm/dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *  include/asm-s390/dma.h
 *
 *  S390 version
 */

#ifndef _ASM_DMA_H
#define _ASM_DMA_H

#include <asm/io.h>		/* need byte IO */

#define MAX_DMA_ADDRESS         0x80000000

#define free_dma(x)	do { } while (0)

#endif /* _ASM_DMA_H */
