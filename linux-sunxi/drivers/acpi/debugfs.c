/*
 * drivers/acpi/debugfs.c
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
 * debugfs.c - ACPI debugfs interface to userspace.
 */

#include <linux/export.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <acpi/acpi_drivers.h>

#define _COMPONENT		ACPI_SYSTEM_COMPONENT
ACPI_MODULE_NAME("debugfs");

struct dentry *acpi_debugfs_dir;
EXPORT_SYMBOL_GPL(acpi_debugfs_dir);

void __init acpi_debugfs_init(void)
{
	acpi_debugfs_dir = debugfs_create_dir("acpi", NULL);
}
