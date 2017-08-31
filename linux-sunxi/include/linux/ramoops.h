/*
 * include/linux/ramoops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __RAMOOPS_H
#define __RAMOOPS_H

/*
 * Ramoops platform data
 * @mem_size	memory size for ramoops
 * @mem_address	physical memory address to contain ramoops
 */

struct ramoops_platform_data {
	unsigned long	mem_size;
	unsigned long	mem_address;
	unsigned long	record_size;
	int		dump_oops;
};

#endif
