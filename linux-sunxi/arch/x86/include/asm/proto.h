/*
 * arch/x86/include/asm/proto.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_PROTO_H
#define _ASM_X86_PROTO_H

#include <asm/ldt.h>

/* misc architecture specific prototypes */

void early_idt_handler(void);

void system_call(void);
void syscall_init(void);

void ia32_syscall(void);
void ia32_cstar_target(void);
void ia32_sysenter_target(void);

void syscall32_cpu_init(void);

void x86_configure_nx(void);
void x86_report_nx(void);

extern int reboot_force;

long do_arch_prctl(struct task_struct *task, int code, unsigned long addr);

#endif /* _ASM_X86_PROTO_H */
