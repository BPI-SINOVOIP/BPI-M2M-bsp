/*
 * arch/sh/mm/uncached.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <asm/sizes.h>
#include <asm/page.h>
#include <asm/addrspace.h>

/*
 * This is the offset of the uncached section from its cached alias.
 *
 * Legacy platforms handle trivial transitions between cached and
 * uncached segments by making use of the 1:1 mapping relationship in
 * 512MB lowmem, others via a special uncached mapping.
 *
 * Default value only valid in 29 bit mode, in 32bit mode this will be
 * updated by the early PMB initialization code.
 */
unsigned long cached_to_uncached = SZ_512M;
unsigned long uncached_size = SZ_512M;
unsigned long uncached_start, uncached_end;
EXPORT_SYMBOL(uncached_start);
EXPORT_SYMBOL(uncached_end);

int virt_addr_uncached(unsigned long kaddr)
{
	return (kaddr >= uncached_start) && (kaddr < uncached_end);
}
EXPORT_SYMBOL(virt_addr_uncached);

void __init uncached_init(void)
{
#if defined(CONFIG_29BIT) || !defined(CONFIG_MMU)
	uncached_start = P2SEG;
#else
	uncached_start = memory_end;
#endif
	uncached_end = uncached_start + uncached_size;
}

void __init uncached_resize(unsigned long size)
{
	uncached_size = size;
	uncached_end = uncached_start + uncached_size;
}
