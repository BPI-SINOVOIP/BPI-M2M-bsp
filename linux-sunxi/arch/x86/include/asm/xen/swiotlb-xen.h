/*
 * arch/x86/include/asm/xen/swiotlb-xen.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_SWIOTLB_XEN_H
#define _ASM_X86_SWIOTLB_XEN_H

#ifdef CONFIG_SWIOTLB_XEN
extern int xen_swiotlb;
extern int __init pci_xen_swiotlb_detect(void);
extern void __init pci_xen_swiotlb_init(void);
#else
#define xen_swiotlb (0)
static inline int __init pci_xen_swiotlb_detect(void) { return 0; }
static inline void __init pci_xen_swiotlb_init(void) { }
#endif

#endif /* _ASM_X86_SWIOTLB_XEN_H */
