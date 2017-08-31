/*
 * arch/score/include/asm/io.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SCORE_IO_H
#define _ASM_SCORE_IO_H

#include <asm-generic/io.h>

#define virt_to_bus	virt_to_phys
#define bus_to_virt	phys_to_virt

#endif /* _ASM_SCORE_IO_H */
