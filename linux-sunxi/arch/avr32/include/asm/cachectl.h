/*
 * arch/avr32/include/asm/cachectl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_CACHECTL_H
#define __ASM_AVR32_CACHECTL_H

/*
 * Operations that can be performed through the cacheflush system call
 */

/* Clean the data cache, then invalidate the icache */
#define CACHE_IFLUSH	0

#endif /* __ASM_AVR32_CACHECTL_H */
