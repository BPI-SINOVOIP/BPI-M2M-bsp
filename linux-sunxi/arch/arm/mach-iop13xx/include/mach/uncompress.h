/*
 * arch/arm/mach-iop13xx/include/mach/uncompress.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/types.h>
#include <linux/serial_reg.h>
#include <mach/hardware.h>

#define UART_BASE ((volatile u32 *)IOP13XX_UART1_PHYS)
#define TX_DONE (UART_LSR_TEMT | UART_LSR_THRE)

static inline void putc(char c)
{
	while ((UART_BASE[UART_LSR] & TX_DONE) != TX_DONE)
		barrier();
	UART_BASE[UART_TX] = c;
}

static inline void flush(void)
{
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
