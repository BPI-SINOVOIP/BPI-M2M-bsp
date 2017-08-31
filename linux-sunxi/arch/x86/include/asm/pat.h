/*
 * arch/x86/include/asm/pat.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_PAT_H
#define _ASM_X86_PAT_H

#include <linux/types.h>
#include <asm/pgtable_types.h>

#ifdef CONFIG_X86_PAT
extern int pat_enabled;
#else
static const int pat_enabled;
#endif

extern void pat_init(void);

extern int reserve_memtype(u64 start, u64 end,
		unsigned long req_type, unsigned long *ret_type);
extern int free_memtype(u64 start, u64 end);

extern int kernel_map_sync_memtype(u64 base, unsigned long size,
		unsigned long flag);

int io_reserve_memtype(resource_size_t start, resource_size_t end,
			unsigned long *type);

void io_free_memtype(resource_size_t start, resource_size_t end);

#endif /* _ASM_X86_PAT_H */
