/*
 * arch/avr32/include/asm/pci.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_PCI_H__
#define __ASM_AVR32_PCI_H__

/* We don't support PCI yet, but some drivers require this file anyway */

#define PCI_DMA_BUS_IS_PHYS	(1)

#include <asm-generic/pci-dma-compat.h>

#endif /* __ASM_AVR32_PCI_H__ */
