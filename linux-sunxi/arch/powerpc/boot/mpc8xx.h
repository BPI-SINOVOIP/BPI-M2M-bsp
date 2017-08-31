/*
 * arch/powerpc/boot/mpc8xx.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PPC_BOOT_MPC8xx_H_
#define _PPC_BOOT_MPC8xx_H_

#include "types.h"

void mpc8xx_set_clocks(u32 sysclk);

u32 mpc885_get_clock(u32 crystal);
int mpc885_fixup_clocks(u32 crystal);

#endif
