/*
 * include/linux/intel_pmic_gpio.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_INTEL_PMIC_H
#define LINUX_INTEL_PMIC_H

struct intel_pmic_gpio_platform_data {
	/* the first IRQ of the chip */
	unsigned	irq_base;
	/* number assigned to the first GPIO */
	unsigned	gpio_base;
	/* sram address for gpiointr register, the langwell chip will map
	 * the PMIC spi GPIO expander's GPIOINTR register in sram.
	 */
	unsigned	gpiointr;
};

#endif
