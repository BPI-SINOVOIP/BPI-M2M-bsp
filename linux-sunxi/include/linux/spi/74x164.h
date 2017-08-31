/*
 * include/linux/spi/74x164.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_SPI_74X164_H
#define LINUX_SPI_74X164_H

struct gen_74x164_chip_platform_data {
	/* number assigned to the first GPIO */
	unsigned	base;
};

#endif
