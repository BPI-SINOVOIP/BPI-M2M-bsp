/*
 * arch/arm/include/asm/nwflash.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _FLASH_H
#define _FLASH_H

#define FLASH_MINOR		 160	/* MAJOR is 10 - miscdevice */
#define CMD_WRITE_DISABLE	 0
#define CMD_WRITE_ENABLE	 0x28
#define CMD_WRITE_BASE64K_ENABLE 0x47

#endif /* _FLASH_H */
