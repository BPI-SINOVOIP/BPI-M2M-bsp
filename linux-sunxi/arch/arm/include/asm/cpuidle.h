/*
 * arch/arm/include/asm/cpuidle.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARM_CPUIDLE_H
#define __ASM_ARM_CPUIDLE_H

#ifdef CONFIG_CPU_IDLE
extern int arm_cpuidle_simple_enter(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index);
#else
static inline int arm_cpuidle_simple_enter(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index) { return -ENODEV; }
#endif

/* Common ARM WFI state */
#define ARM_CPUIDLE_WFI_STATE_PWR(p) {\
	.enter                  = arm_cpuidle_simple_enter,\
	.exit_latency           = 1,\
	.target_residency       = 1,\
	.power_usage		= p,\
	.flags                  = CPUIDLE_FLAG_TIME_VALID,\
	.name                   = "WFI",\
	.desc                   = "ARM WFI",\
}

/*
 * in case power_specified == 1, give a default WFI power value needed
 * by some governors
 */
#define ARM_CPUIDLE_WFI_STATE ARM_CPUIDLE_WFI_STATE_PWR(UINT_MAX)

#endif
