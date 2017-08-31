/*
 * include/linux/i2c/mpr121_touchkey.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Header file for Freescale MPR121 Capacitive Touch Sensor */

#ifndef _MPR121_TOUCHKEY_H
#define _MPR121_TOUCHKEY_H

/**
 * struct mpr121_platform_data - platform data for mpr121 sensor
 * @keymap: pointer to array of KEY_* values representing keymap
 * @keymap_size: size of the keymap
 * @wakeup: configure the button as a wake-up source
 * @vdd_uv: VDD voltage in uV
 */
struct mpr121_platform_data {
	const unsigned short *keymap;
	unsigned int keymap_size;
	bool wakeup;
	int vdd_uv;
};

#endif /* _MPR121_TOUCHKEY_H */
