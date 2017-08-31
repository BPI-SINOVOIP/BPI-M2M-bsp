/*
 * tools/perf/util/include/asm/uaccess.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_ASM_UACCESS_H_
#define _PERF_ASM_UACCESS_H_

#define __get_user(src, dest)						\
({									\
	(src) = *dest;							\
	0;								\
})

#define get_user	__get_user

#define access_ok(type, addr, size)	1

#endif
