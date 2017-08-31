/*
 * arch/arm/include/asm/suspend.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARM_SUSPEND_H
#define __ASM_ARM_SUSPEND_H

struct sleep_save_sp {
	u32 *save_ptr_stash;
	u32 save_ptr_stash_phys;
};

extern void cpu_resume(void);
extern int cpu_suspend(unsigned long, int (*)(unsigned long));

#endif
