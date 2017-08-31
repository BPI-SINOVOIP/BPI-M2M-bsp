/*
 * arch/sh/include/asm/cachectl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SH_CACHECTL_H
#define _SH_CACHECTL_H

/* Definitions for the cacheflush system call.  */

#define CACHEFLUSH_D_INVAL	0x1	/* invalidate (without write back) */
#define CACHEFLUSH_D_WB		0x2	/* write back (without invalidate) */
#define CACHEFLUSH_D_PURGE	0x3	/* writeback and invalidate */

#define CACHEFLUSH_I		0x4

/*
 * Options for cacheflush system call
 */
#define ICACHE	CACHEFLUSH_I		/* flush instruction cache */
#define DCACHE	CACHEFLUSH_D_PURGE	/* writeback and flush data cache */
#define BCACHE	(ICACHE|DCACHE)		/* flush both caches */

#endif /* _SH_CACHECTL_H */
