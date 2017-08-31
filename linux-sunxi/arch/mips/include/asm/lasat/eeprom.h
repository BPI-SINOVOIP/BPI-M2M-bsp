/*
 * arch/mips/include/asm/lasat/eeprom.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/addrspace.h>

/* lasat 100 */
#define AT93C_REG_100               KSEG1ADDR(0x1c810000)
#define AT93C_RDATA_REG_100         AT93C_REG_100
#define AT93C_RDATA_SHIFT_100       4
#define AT93C_WDATA_SHIFT_100       4
#define AT93C_CS_M_100              (1 << 5)
#define AT93C_CLK_M_100             (1 << 3)

/* lasat 200 */
#define AT93C_REG_200		KSEG1ADDR(0x11000000)
#define AT93C_RDATA_REG_200	(AT93C_REG_200+0x10000)
#define AT93C_RDATA_SHIFT_200	8
#define AT93C_WDATA_SHIFT_200	2
#define AT93C_CS_M_200		(1 << 0)
#define AT93C_CLK_M_200		(1 << 1)
