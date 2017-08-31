/*
 * arch/microblaze/include/asm/syscalls.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MICROBLAZE_SYSCALLS_H

asmlinkage long microblaze_vfork(struct pt_regs *regs);
asmlinkage long microblaze_clone(int flags, unsigned long stack,
							struct pt_regs *regs);
asmlinkage long microblaze_execve(const char __user *filenamei,
				  const char __user *const __user *argv,
				  const char __user *const __user *envp,
				  struct pt_regs *regs);

asmlinkage long sys_clone(int flags, unsigned long stack, struct pt_regs *regs);
#define sys_clone sys_clone

#include <asm-generic/syscalls.h>

#endif /* __ASM_MICROBLAZE_SYSCALLS_H */
