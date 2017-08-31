/*
 * arch/sh/include/mach-se/mach/se7206.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_SE7206_H
#define __ASM_SH_SE7206_H

#define PA_SMSC		0x30000000
#define PA_MRSHPC	0x34000000
#define PA_LED		0x31400000

void init_se7206_IRQ(void);

#define __IO_PREFIX	se7206
#include <asm/io_generic.h>

#endif /* __ASM_SH_SE7206_H */
