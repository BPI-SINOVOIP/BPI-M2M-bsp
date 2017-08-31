/*
 * arch/arm/mach-ep93xx/include/mach/memory.h
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
 * arch/arm/mach-ep93xx/include/mach/memory.h
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#if defined(CONFIG_EP93XX_SDCE3_SYNC_PHYS_OFFSET)
#define PLAT_PHYS_OFFSET		UL(0x00000000)
#elif defined(CONFIG_EP93XX_SDCE0_PHYS_OFFSET)
#define PLAT_PHYS_OFFSET		UL(0xc0000000)
#elif defined(CONFIG_EP93XX_SDCE1_PHYS_OFFSET)
#define PLAT_PHYS_OFFSET		UL(0xd0000000)
#elif defined(CONFIG_EP93XX_SDCE2_PHYS_OFFSET)
#define PLAT_PHYS_OFFSET		UL(0xe0000000)
#elif defined(CONFIG_EP93XX_SDCE3_ASYNC_PHYS_OFFSET)
#define PLAT_PHYS_OFFSET		UL(0xf0000000)
#else
#error "Kconfig bug: No EP93xx PHYS_OFFSET set"
#endif

#endif
