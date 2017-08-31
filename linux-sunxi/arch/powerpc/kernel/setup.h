/*
 * arch/powerpc/kernel/setup.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _POWERPC_KERNEL_SETUP_H
#define _POWERPC_KERNEL_SETUP_H

void check_for_initrd(void);
void do_init_bootmem(void);
void setup_panic(void);
extern int do_early_xmon;

#endif /* _POWERPC_KERNEL_SETUP_H */
