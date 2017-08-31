/*
 * arch/x86/include/asm/mmconfig.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_MMCONFIG_H
#define _ASM_X86_MMCONFIG_H

#ifdef CONFIG_PCI_MMCONFIG
extern void __cpuinit fam10h_check_enable_mmcfg(void);
extern void __cpuinit check_enable_amd_mmconf_dmi(void);
#else
static inline void fam10h_check_enable_mmcfg(void) { }
static inline void check_enable_amd_mmconf_dmi(void) { }
#endif

#endif /* _ASM_X86_MMCONFIG_H */
