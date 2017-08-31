/*
 * arch/unicore32/include/mach/regs-ps2.h
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
 * PKUnity PS2 Controller Registers
 */
/*
 * the same as I8042_DATA_REG PS2_DATA
 */
#define PS2_DATA	(PKUNITY_PS2_BASE + 0x0060)
/*
 * the same as I8042_COMMAND_REG PS2_COMMAND
 */
#define PS2_COMMAND	(PKUNITY_PS2_BASE + 0x0064)
/*
 * the same as I8042_STATUS_REG PS2_STATUS
 */
#define PS2_STATUS	(PKUNITY_PS2_BASE + 0x0064)
/*
 * counter reg PS2_CNT
 */
#define PS2_CNT		(PKUNITY_PS2_BASE + 0x0068)

