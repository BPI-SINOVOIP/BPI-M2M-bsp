/*
 * arch/s390/include/asm/pci.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_S390_PCI_H
#define __ASM_S390_PCI_H

/* S/390 systems don't have a PCI bus. This file is just here because some stupid .c code
 * includes it even if CONFIG_PCI is not set.
 */
#define PCI_DMA_BUS_IS_PHYS (0)

#endif /* __ASM_S390_PCI_H */

