/*
 * arch/x86/kernel/cpu/powerflags.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Strings for the various x86 power flags
 *
 * This file must not contain any executable code.
 */

#include <asm/cpufeature.h>

const char *const x86_power_flags[32] = {
	"ts",	/* temperature sensor */
	"fid",  /* frequency id control */
	"vid",  /* voltage id control */
	"ttp",  /* thermal trip */
	"tm",
	"stc",
	"100mhzsteps",
	"hwpstate",
	"",	/* tsc invariant mapped to constant_tsc */
	"cpb",  /* core performance boost */
	"eff_freq_ro", /* Readonly aperf/mperf */
};
