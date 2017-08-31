/*
 * include/linux/gpio-pxa.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __GPIO_PXA_H
#define __GPIO_PXA_H

#define GPIO_bit(x)	(1 << ((x) & 0x1f))

#define gpio_to_bank(gpio)	((gpio) >> 5)

/* NOTE: some PXAs have fewer on-chip GPIOs (like PXA255, with 85).
 * Those cases currently cause holes in the GPIO number space, the
 * actual number of the last GPIO is recorded by 'pxa_last_gpio'.
 */
extern int pxa_last_gpio;

extern int pxa_irq_to_gpio(int irq);

struct pxa_gpio_platform_data {
	int (*gpio_set_wake)(unsigned int gpio, unsigned int on);
};

#endif /* __GPIO_PXA_H */
