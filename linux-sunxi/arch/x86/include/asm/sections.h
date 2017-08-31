/*
 * arch/x86/include/asm/sections.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_SECTIONS_H
#define _ASM_X86_SECTIONS_H

#include <asm-generic/sections.h>
#include <asm/uaccess.h>

extern char __brk_base[], __brk_limit[];
extern struct exception_table_entry __stop___ex_table[];

#if defined(CONFIG_X86_64) && defined(CONFIG_DEBUG_RODATA)
extern char __end_rodata_hpage_align[];
#endif

#endif	/* _ASM_X86_SECTIONS_H */
