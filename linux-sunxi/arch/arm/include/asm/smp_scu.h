/*
 * arch/arm/include/asm/smp_scu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASMARM_ARCH_SCU_H
#define __ASMARM_ARCH_SCU_H

#define SCU_PM_NORMAL	0
#define SCU_PM_DORMANT	2
#define SCU_PM_POWEROFF	3

#ifndef __ASSEMBLER__
unsigned int scu_get_core_count(void __iomem *);
void scu_enable(void __iomem *);
int scu_power_mode(void __iomem *, unsigned int);
#endif

#endif
