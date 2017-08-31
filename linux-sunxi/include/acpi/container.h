/*
 * include/acpi/container.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ACPI_CONTAINER_H
#define __ACPI_CONTAINER_H

#include <linux/kernel.h>

struct acpi_container {
	acpi_handle handle;
	unsigned long sun;
	int state;
};

#endif				/* __ACPI_CONTAINER_H */
