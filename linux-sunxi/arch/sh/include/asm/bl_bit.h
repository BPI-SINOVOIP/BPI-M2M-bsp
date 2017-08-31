/*
 * arch/sh/include/asm/bl_bit.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_BL_BIT_H
#define __ASM_SH_BL_BIT_H

#ifdef CONFIG_SUPERH32
# include "bl_bit_32.h"
#else
# include "bl_bit_64.h"
#endif

#endif /* __ASM_SH_BL_BIT_H */
