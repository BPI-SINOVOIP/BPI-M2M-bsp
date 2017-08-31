/*
 * tools/perf/util/include/asm/bug.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_ASM_GENERIC_BUG_H
#define _PERF_ASM_GENERIC_BUG_H

#define __WARN_printf(arg...)	do { fprintf(stderr, arg); } while (0)

#define WARN(condition, format...) ({		\
	int __ret_warn_on = !!(condition);	\
	if (unlikely(__ret_warn_on))		\
		__WARN_printf(format);		\
	unlikely(__ret_warn_on);		\
})

#define WARN_ONCE(condition, format...)	({	\
	static int __warned;			\
	int __ret_warn_once = !!(condition);	\
						\
	if (unlikely(__ret_warn_once))		\
		if (WARN(!__warned, format)) 	\
			__warned = 1;		\
	unlikely(__ret_warn_once);		\
})
#endif
