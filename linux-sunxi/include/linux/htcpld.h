/*
 * include/linux/htcpld.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_HTCPLD_H
#define __LINUX_HTCPLD_H

struct htcpld_chip_platform_data {
	unsigned int addr;
	unsigned int reset;
	unsigned int num_gpios;
	unsigned int gpio_out_base;
	unsigned int gpio_in_base;
	unsigned int irq_base;
	unsigned int num_irqs;
};

struct htcpld_core_platform_data {
	unsigned int                      int_reset_gpio_hi;
	unsigned int                      int_reset_gpio_lo;
	unsigned int                      i2c_adapter_id;

	struct htcpld_chip_platform_data  *chip;
	unsigned int                      num_chip;
};

#endif /* __LINUX_HTCPLD_H */

