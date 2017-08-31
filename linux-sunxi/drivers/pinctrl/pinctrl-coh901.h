/*
 * drivers/pinctrl/pinctrl-coh901.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
int u300_gpio_config_get(struct gpio_chip *chip,
			 unsigned offset,
			 unsigned long *config);
int u300_gpio_config_set(struct gpio_chip *chip, unsigned offset,
			 enum pin_config_param param);
