/*
 * arch/mips/include/asm/mach-bcm63xx/gpio.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MIPS_MACH_BCM63XX_GPIO_H
#define __ASM_MIPS_MACH_BCM63XX_GPIO_H

#include <bcm63xx_gpio.h>

#define gpio_to_irq(gpio)	-1

#define gpio_get_value __gpio_get_value
#define gpio_set_value __gpio_set_value

#define gpio_cansleep __gpio_cansleep

#include <asm-generic/gpio.h>

#endif /* __ASM_MIPS_MACH_BCM63XX_GPIO_H */
