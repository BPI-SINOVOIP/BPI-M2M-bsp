/*
 * arch/mips/include/asm/mach-tx49xx/cpu-feature-overrides.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MACH_TX49XX_CPU_FEATURE_OVERRIDES_H
#define __ASM_MACH_TX49XX_CPU_FEATURE_OVERRIDES_H

#define cpu_has_llsc	1
#define cpu_has_64bits	1
#define cpu_has_inclusive_pcaches	0

#define cpu_has_mips16		0
#define cpu_has_mdmx		0
#define cpu_has_mips3d		0
#define cpu_has_smartmips	0
#define cpu_has_vtag_icache	0
#define cpu_has_ic_fills_f_dc	0
#define cpu_has_dsp	0
#define cpu_has_mipsmt	0
#define cpu_has_userlocal	0

#define cpu_has_mips32r1	0
#define cpu_has_mips32r2	0
#define cpu_has_mips64r1	0
#define cpu_has_mips64r2	0

#endif /* __ASM_MACH_TX49XX_CPU_FEATURE_OVERRIDES_H */
