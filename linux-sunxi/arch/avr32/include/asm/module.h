/*
 * arch/avr32/include/asm/module.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_MODULE_H
#define __ASM_AVR32_MODULE_H

struct mod_arch_syminfo {
	unsigned long got_offset;
	int got_initialized;
};

struct mod_arch_specific {
	/* Starting offset of got in the module core memory. */
	unsigned long got_offset;
	/* Size of the got. */
	unsigned long got_size;
	/* Number of symbols in syminfo. */
	int nsyms;
	/* Additional symbol information (got offsets). */
	struct mod_arch_syminfo *syminfo;
};

#define Elf_Shdr		Elf32_Shdr
#define Elf_Sym			Elf32_Sym
#define Elf_Ehdr		Elf32_Ehdr

#define MODULE_PROC_FAMILY "AVR32v1"

#define MODULE_ARCH_VERMAGIC MODULE_PROC_FAMILY

#endif /* __ASM_AVR32_MODULE_H */
