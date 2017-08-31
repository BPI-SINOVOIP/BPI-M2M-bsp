/*
 * include/linux/eeprom_93xx46.h
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
 * Module: eeprom_93xx46
 * platform description for 93xx46 EEPROMs.
 */

struct eeprom_93xx46_platform_data {
	unsigned char	flags;
#define EE_ADDR8	0x01		/*  8 bit addr. cfg */
#define EE_ADDR16	0x02		/* 16 bit addr. cfg */
#define EE_READONLY	0x08		/* forbid writing */

	/*
	 * optional hooks to control additional logic
	 * before and after spi transfer.
	 */
	void (*prepare)(void *);
	void (*finish)(void *);
};
