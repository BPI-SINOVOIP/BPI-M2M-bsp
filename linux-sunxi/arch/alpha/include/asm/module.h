/*
 * arch/alpha/include/asm/module.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ALPHA_MODULE_H
#define _ALPHA_MODULE_H

struct mod_arch_specific
{
	unsigned int gotsecindex;
};

#define Elf_Sym Elf64_Sym
#define Elf_Shdr Elf64_Shdr
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#define Elf_Dyn Elf64_Dyn
#define Elf_Rel Elf64_Rel
#define Elf_Rela Elf64_Rela

#define ARCH_SHF_SMALL SHF_ALPHA_GPREL

#ifdef MODULE
asm(".section .got,\"aws\",@progbits; .align 3; .previous");
#endif

#endif /*_ALPHA_MODULE_H*/
