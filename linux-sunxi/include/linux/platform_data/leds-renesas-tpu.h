/*
 * include/linux/platform_data/leds-renesas-tpu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LEDS_RENESAS_TPU_H__
#define __LEDS_RENESAS_TPU_H__

struct led_renesas_tpu_config {
	char *name;
	unsigned pin_gpio_fn;
	unsigned pin_gpio;
	unsigned int channel_offset;
	unsigned int timer_bit;
	unsigned int max_brightness;
	unsigned int refresh_rate;
};

#endif /* __LEDS_RENESAS_TPU_H__ */
