/*
 * tools/perf/arch/powerpc/util/header.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../util/header.h"

#define __stringify_1(x)        #x
#define __stringify(x)          __stringify_1(x)

#define mfspr(rn)       ({unsigned long rval; \
			 asm volatile("mfspr %0," __stringify(rn) \
				      : "=r" (rval)); rval; })

#define SPRN_PVR        0x11F	/* Processor Version Register */
#define PVR_VER(pvr)    (((pvr) >>  16) & 0xFFFF) /* Version field */
#define PVR_REV(pvr)    (((pvr) >>   0) & 0xFFFF) /* Revison field */

int
get_cpuid(char *buffer, size_t sz)
{
	unsigned long pvr;
	int nb;

	pvr = mfspr(SPRN_PVR);

	nb = scnprintf(buffer, sz, "%lu,%lu$", PVR_VER(pvr), PVR_REV(pvr));

	/* look for end marker to ensure the entire data fit */
	if (strchr(buffer, '$')) {
		buffer[nb-1] = '\0';
		return 0;
	}
	return -1;
}
