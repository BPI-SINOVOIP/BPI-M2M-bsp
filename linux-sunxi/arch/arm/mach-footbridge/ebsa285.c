/*
 * arch/arm/mach-footbridge/ebsa285.c
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
 * linux/arch/arm/mach-footbridge/ebsa285.c
 *
 * EBSA285 machine fixup
 */
#include <linux/init.h>
#include <linux/spinlock.h>

#include <asm/hardware/dec21285.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>

#include "common.h"

MACHINE_START(EBSA285, "EBSA285")
	/* Maintainer: Russell King */
	.atag_offset	= 0x100,
	.video_start	= 0x000a0000,
	.video_end	= 0x000bffff,
	.map_io		= footbridge_map_io,
	.init_irq	= footbridge_init_irq,
	.timer		= &footbridge_timer,
	.restart	= footbridge_restart,
MACHINE_END

