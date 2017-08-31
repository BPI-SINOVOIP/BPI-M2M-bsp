/*
 * arch/powerpc/kernel/cacheinfo.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PPC_CACHEINFO_H
#define _PPC_CACHEINFO_H

/* These are just hooks for sysfs.c to use. */
extern void cacheinfo_cpu_online(unsigned int cpu_id);
extern void cacheinfo_cpu_offline(unsigned int cpu_id);

#endif /* _PPC_CACHEINFO_H */
