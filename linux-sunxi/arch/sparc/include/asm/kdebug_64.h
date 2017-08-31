/*
 * arch/sparc/include/asm/kdebug_64.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SPARC64_KDEBUG_H
#define _SPARC64_KDEBUG_H

struct pt_regs;

extern void bad_trap(struct pt_regs *, long);

/* Grossly misnamed. */
enum die_val {
	DIE_OOPS = 1,
	DIE_DEBUG,	/* ta 0x70 */
	DIE_DEBUG_2,	/* ta 0x71 */
	DIE_DIE,
	DIE_TRAP,
	DIE_TRAP_TL1,
	DIE_CALL,
	DIE_NMI,
	DIE_NMIWATCHDOG,
};

#endif
