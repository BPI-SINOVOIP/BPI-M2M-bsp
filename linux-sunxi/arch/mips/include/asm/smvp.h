/*
 * arch/mips/include/asm/smvp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SMVP_H
#define _ASM_SMVP_H

/*
 * Definitions for SMVP multitasking on MIPS MT cores
 */
struct task_struct;

extern void smvp_smp_setup(void);
extern void smvp_smp_finish(void);
extern void smvp_boot_secondary(int cpu, struct task_struct *t);
extern void smvp_init_secondary(void);
extern void smvp_smp_finish(void);
extern void smvp_cpus_done(void);
extern void smvp_prepare_cpus(unsigned int max_cpus);

/* This is platform specific */
extern void smvp_send_ipi(int cpu, unsigned int action);
#endif /*  _ASM_SMVP_H */
