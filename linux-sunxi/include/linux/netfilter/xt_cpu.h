/*
 * include/linux/netfilter/xt_cpu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_CPU_H
#define _XT_CPU_H

#include <linux/types.h>

struct xt_cpu_info {
	__u32	cpu;
	__u32	invert;
};

#endif /*_XT_CPU_H*/
