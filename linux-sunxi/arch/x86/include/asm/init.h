/*
 * arch/x86/include/asm/init.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_INIT_32_H
#define _ASM_X86_INIT_32_H

#ifdef CONFIG_X86_32
extern void __init early_ioremap_page_table_range_init(void);
#endif

extern void __init zone_sizes_init(void);

extern unsigned long __init
kernel_physical_mapping_init(unsigned long start,
			     unsigned long end,
			     unsigned long page_size_mask);


extern unsigned long __initdata pgt_buf_start;
extern unsigned long __meminitdata pgt_buf_end;
extern unsigned long __meminitdata pgt_buf_top;

#endif /* _ASM_X86_INIT_32_H */
