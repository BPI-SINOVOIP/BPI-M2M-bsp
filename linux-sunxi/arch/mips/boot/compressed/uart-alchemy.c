/*
 * arch/mips/boot/compressed/uart-alchemy.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/mach-au1x00/au1000.h>

void putc(char c)
{
#ifdef CONFIG_MIPS_DB1300
	alchemy_uart_putchar(AU1300_UART2_PHYS_ADDR, c);
#else
	alchemy_uart_putchar(AU1000_UART0_PHYS_ADDR, c);
#endif
}
