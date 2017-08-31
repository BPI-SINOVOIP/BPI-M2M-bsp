/*
 * arch/cris/include/asm/module.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_CRIS_MODULE_H
#define _ASM_CRIS_MODULE_H
/* cris is simple */
struct mod_arch_specific { };

#define Elf_Shdr Elf32_Shdr
#define Elf_Sym Elf32_Sym
#define Elf_Ehdr Elf32_Ehdr
#endif /* _ASM_CRIS_MODULE_H */
