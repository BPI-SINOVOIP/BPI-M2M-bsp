/*
 * arch/arm/plat-omap/include/plat/omap-secure.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __OMAP_SECURE_H__
#define __OMAP_SECURE_H__

#include <linux/types.h>

#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_OMAP4)
extern int omap_secure_ram_reserve_memblock(void);
#else
static inline void omap_secure_ram_reserve_memblock(void)
{ }
#endif

#ifdef CONFIG_OMAP4_ERRATA_I688
extern int omap_barrier_reserve_memblock(void);
#else
static inline void omap_barrier_reserve_memblock(void)
{ }
#endif
#endif /* __OMAP_SECURE_H__ */
