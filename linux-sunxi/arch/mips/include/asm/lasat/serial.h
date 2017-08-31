/*
 * arch/mips/include/asm/lasat/serial.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/lasat/lasat.h>

/* Lasat 100 boards serial configuration */
#define LASAT_BASE_BAUD_100 		(7372800 / 16)
#define LASAT_UART_REGS_BASE_100	0x1c8b0000
#define LASAT_UART_REGS_SHIFT_100	2
#define LASATINT_UART_100		16

/* * LASAT 200 boards serial configuration */
#define LASAT_BASE_BAUD_200		(100000000 / 16 / 12)
#define LASAT_UART_REGS_BASE_200	(Vrc5074_PHYS_BASE + 0x0300)
#define LASAT_UART_REGS_SHIFT_200	3
#define LASATINT_UART_200		21
