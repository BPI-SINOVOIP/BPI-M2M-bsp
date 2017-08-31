/*
 * arch/x86/um/tls_64.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "linux/sched.h"

void clear_flushed_tls(struct task_struct *task)
{
}

int arch_copy_tls(struct task_struct *t)
{
	/*
	 * If CLONE_SETTLS is set, we need to save the thread id
	 * (which is argument 5, child_tid, of clone) so it can be set
	 * during context switches.
	 */
	t->thread.arch.fs = t->thread.regs.regs.gp[R8 / sizeof(long)];

	return 0;
}
