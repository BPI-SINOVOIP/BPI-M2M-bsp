/*
 * arch/x86/include/asm/cpumask.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_CPUMASK_H
#define _ASM_X86_CPUMASK_H
#ifndef __ASSEMBLY__
#include <linux/cpumask.h>

extern cpumask_var_t cpu_callin_mask;
extern cpumask_var_t cpu_callout_mask;
extern cpumask_var_t cpu_initialized_mask;
extern cpumask_var_t cpu_sibling_setup_mask;

extern void setup_cpu_local_masks(void);

#endif /* __ASSEMBLY__ */
#endif /* _ASM_X86_CPUMASK_H */
