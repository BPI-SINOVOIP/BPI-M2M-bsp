/*
 * arch/h8300/include/asm/cachectl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _H8300_CACHECTL_H
#define _H8300_CACHECTL_H

/* Definitions for the cacheflush system call.  */

#define FLUSH_SCOPE_LINE    0	/* Flush a cache line */
#define FLUSH_SCOPE_PAGE    0	/* Flush a page */
#define FLUSH_SCOPE_ALL     0	/* Flush the whole cache -- superuser only */

#define FLUSH_CACHE_DATA    0	/* Writeback and flush data cache */
#define FLUSH_CACHE_INSN    0	/* Flush instruction cache */
#define FLUSH_CACHE_BOTH    0	/* Flush both caches */

#endif /* _H8300_CACHECTL_H */
