/*
 * arch/arm/mach-vt8500/include/mach/system.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * arch/arm/mach-vt8500/include/mach/system.h
 *
 */
#include <asm/io.h>

/* PM Software Reset request register */
#define VT8500_PMSR_VIRT	0xf8130060

static inline void arch_reset(char mode, const char *cmd)
{
	writel(1, VT8500_PMSR_VIRT);
}
