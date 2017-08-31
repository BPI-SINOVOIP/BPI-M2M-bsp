/*
 * include/asm-generic/module.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_MODULE_H
#define __ASM_GENERIC_MODULE_H

/*
 * Many architectures just need a simple module
 * loader without arch specific data.
 */
struct mod_arch_specific
{
};

#ifdef CONFIG_64BIT
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym Elf64_Sym
#define Elf_Ehdr Elf64_Ehdr
#else
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym Elf32_Sym
#define Elf_Ehdr Elf32_Ehdr
#endif

#endif /* __ASM_GENERIC_MODULE_H */
