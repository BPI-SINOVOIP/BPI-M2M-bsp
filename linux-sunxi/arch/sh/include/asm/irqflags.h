/*
 * arch/sh/include/asm/irqflags.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_IRQFLAGS_H
#define __ASM_SH_IRQFLAGS_H

#define ARCH_IRQ_DISABLED	0xf0
#define ARCH_IRQ_ENABLED	0x00

#include <asm-generic/irqflags.h>

#endif /* __ASM_SH_IRQFLAGS_H */
