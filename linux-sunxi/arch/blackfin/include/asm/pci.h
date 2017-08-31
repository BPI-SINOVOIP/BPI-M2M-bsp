/*
 * arch/blackfin/include/asm/pci.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Changed from asm-m68k version, Lineo Inc. 	May 2001	*/

#ifndef _ASM_BFIN_PCI_H
#define _ASM_BFIN_PCI_H

#include <asm/scatterlist.h>
#include <asm-generic/pci-dma-compat.h>
#include <asm-generic/pci.h>

#define PCIBIOS_MIN_IO 0x00001000
#define PCIBIOS_MIN_MEM 0x10000000

static inline void pcibios_penalize_isa_irq(int irq)
{
	/* We don't do dynamic PCI IRQ allocation */
}

#endif				/* _ASM_BFIN_PCI_H */
