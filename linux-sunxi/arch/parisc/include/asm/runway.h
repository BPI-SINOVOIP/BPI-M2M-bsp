/*
 * arch/parisc/include/asm/runway.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ASM_PARISC_RUNWAY_H
#define ASM_PARISC_RUNWAY_H
#ifdef __KERNEL__

/* declared in arch/parisc/kernel/setup.c */
extern struct proc_dir_entry * proc_runway_root;

#define RUNWAY_STATUS	0x10
#define RUNWAY_DEBUG	0x40

#endif /* __KERNEL__ */
#endif /* ASM_PARISC_RUNWAY_H */
