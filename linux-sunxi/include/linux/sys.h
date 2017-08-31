/*
 * include/linux/sys.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_SYS_H
#define _LINUX_SYS_H

/*
 * This file is no longer used or needed
 */

/*
 * These are system calls that will be removed at some time
 * due to newer versions existing..
 * (please be careful - ibcs2 may need some of these).
 */
#ifdef notdef
#define _sys_waitpid	_sys_old_syscall	/* _sys_wait4 */
#define _sys_olduname	_sys_old_syscall	/* _sys_newuname */
#define _sys_uname	_sys_old_syscall	/* _sys_newuname */
#define _sys_stat	_sys_old_syscall	/* _sys_newstat */
#define _sys_fstat	_sys_old_syscall	/* _sys_newfstat */
#define _sys_lstat	_sys_old_syscall	/* _sys_newlstat */
#define _sys_signal	_sys_old_syscall	/* _sys_sigaction */
#define _sys_sgetmask	_sys_old_syscall	/* _sys_sigprocmask */
#define _sys_ssetmask	_sys_old_syscall	/* _sys_sigprocmask */
#endif

/*
 * These are system calls that haven't been implemented yet
 * but have an entry in the table for future expansion..
 */
#endif
