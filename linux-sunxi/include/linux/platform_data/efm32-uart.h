/*
 * include/linux/platform_data/efm32-uart.h
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
 *
 *
 */
#ifndef __LINUX_PLATFORM_DATA_EFM32_UART_H__
#define __LINUX_PLATFORM_DATA_EFM32_UART_H__

#include <linux/types.h>

/**
 * struct efm32_uart_pdata
 * @location: pinmux location for the I/O pins (to be written to the ROUTE
 * 	register)
 */
struct efm32_uart_pdata {
	u8 location;
};
#endif /* ifndef __LINUX_PLATFORM_DATA_EFM32_UART_H__ */
