/*
 * include/linux/sysinfo.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_SYSINFO_H
#define _LINUX_SYSINFO_H

#include <linux/types.h>

#define SI_LOAD_SHIFT	16
struct sysinfo {
	__kernel_long_t uptime;		/* Seconds since boot */
	__kernel_ulong_t loads[3];	/* 1, 5, and 15 minute load averages */
	__kernel_ulong_t totalram;	/* Total usable main memory size */
	__kernel_ulong_t freeram;	/* Available memory size */
	__kernel_ulong_t sharedram;	/* Amount of shared memory */
	__kernel_ulong_t bufferram;	/* Memory used by buffers */
	__kernel_ulong_t totalswap;	/* Total swap space size */
	__kernel_ulong_t freeswap;	/* swap space still available */
	__u16 procs;		   	/* Number of current processes */
	__u16 pad;		   	/* Explicit padding for m68k */
	__kernel_ulong_t totalhigh;	/* Total high memory size */
	__kernel_ulong_t freehigh;	/* Available high memory size */
	__u32 mem_unit;			/* Memory unit size in bytes */
	char _f[20-2*sizeof(__kernel_ulong_t)-sizeof(__u32)];	/* Padding: libc5 uses this.. */
};

#endif /* _LINUX_SYSINFO_H */
