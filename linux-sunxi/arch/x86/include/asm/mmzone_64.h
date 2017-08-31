/*
 * arch/x86/include/asm/mmzone_64.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* K8 NUMA support */
/* Copyright 2002,2003 by Andi Kleen, SuSE Labs */
/* 2.5 Version loosely based on the NUMAQ Code by Pat Gaughen. */
#ifndef _ASM_X86_MMZONE_64_H
#define _ASM_X86_MMZONE_64_H

#ifdef CONFIG_NUMA

#include <linux/mmdebug.h>
#include <asm/smp.h>

extern struct pglist_data *node_data[];

#define NODE_DATA(nid)		(node_data[nid])

#endif
#endif /* _ASM_X86_MMZONE_64_H */
