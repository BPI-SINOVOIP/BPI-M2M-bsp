/*
 * arch/m68k/include/asm/current.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _M68K_CURRENT_H
#define _M68K_CURRENT_H

#ifdef CONFIG_MMU

register struct task_struct *current __asm__("%a2");

#else

/*
 *	Rather than dedicate a register (as the m68k source does), we
 *	just keep a global,  we should probably just change it all to be
 *	current and lose _current_task.
 */
#include <linux/thread_info.h>

struct task_struct;

static inline struct task_struct *get_current(void)
{
	return(current_thread_info()->task);
}

#define	current	get_current()

#endif /* CONFNIG_MMU */

#endif /* !(_M68K_CURRENT_H) */
