/*
 * arch/avr32/include/asm/kdebug.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_KDEBUG_H
#define __ASM_AVR32_KDEBUG_H

/* Grossly misnamed. */
enum die_val {
	DIE_BREAKPOINT,
	DIE_SSTEP,
	DIE_NMI,
	DIE_OOPS,
};

#endif /* __ASM_AVR32_KDEBUG_H */
