/*
 * arch/x86/mm/numa_internal.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __X86_MM_NUMA_INTERNAL_H
#define __X86_MM_NUMA_INTERNAL_H

#include <linux/types.h>
#include <asm/numa.h>

struct numa_memblk {
	u64			start;
	u64			end;
	int			nid;
};

struct numa_meminfo {
	int			nr_blks;
	struct numa_memblk	blk[NR_NODE_MEMBLKS];
};

void __init numa_remove_memblk_from(int idx, struct numa_meminfo *mi);
int __init numa_cleanup_meminfo(struct numa_meminfo *mi);
void __init numa_reset_distance(void);

void __init x86_numa_init(void);

#ifdef CONFIG_NUMA_EMU
void __init numa_emulation(struct numa_meminfo *numa_meminfo,
			   int numa_dist_cnt);
#else
static inline void numa_emulation(struct numa_meminfo *numa_meminfo,
				  int numa_dist_cnt)
{ }
#endif

#endif	/* __X86_MM_NUMA_INTERNAL_H */
