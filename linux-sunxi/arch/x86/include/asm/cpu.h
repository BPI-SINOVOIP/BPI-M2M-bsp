/*
 * arch/x86/include/asm/cpu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_CPU_H
#define _ASM_X86_CPU_H

#include <linux/device.h>
#include <linux/cpu.h>
#include <linux/topology.h>
#include <linux/nodemask.h>
#include <linux/percpu.h>

#ifdef CONFIG_SMP

extern void prefill_possible_map(void);

#else /* CONFIG_SMP */

static inline void prefill_possible_map(void) {}

#define cpu_physical_id(cpu)			boot_cpu_physical_apicid
#define safe_smp_processor_id()			0
#define stack_smp_processor_id()		0

#endif /* CONFIG_SMP */

struct x86_cpu {
	struct cpu cpu;
};

#ifdef CONFIG_HOTPLUG_CPU
extern int arch_register_cpu(int num);
extern void arch_unregister_cpu(int);
#endif

DECLARE_PER_CPU(int, cpu_state);

int mwait_usable(const struct cpuinfo_x86 *);

#endif /* _ASM_X86_CPU_H */
