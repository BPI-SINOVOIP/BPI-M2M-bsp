/*
 * arch/sh/include/asm/unwinder.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_UNWINDER_H
#define _LINUX_UNWINDER_H

#include <asm/stacktrace.h>

struct unwinder {
	const char *name;
	struct list_head list;
	int rating;
	void (*dump)(struct task_struct *, struct pt_regs *,
		     unsigned long *, const struct stacktrace_ops *, void *);
};

extern int unwinder_init(void);
extern int unwinder_register(struct unwinder *);

extern void unwind_stack(struct task_struct *, struct pt_regs *,
			 unsigned long *, const struct stacktrace_ops *,
			 void *);

extern void stack_reader_dump(struct task_struct *, struct pt_regs *,
			      unsigned long *, const struct stacktrace_ops *,
			      void *);

/*
 * Used by fault handling code to signal to the unwinder code that it
 * should switch to a different unwinder.
 */
extern int unwinder_faulted;

#endif /* _LINUX_UNWINDER_H */
