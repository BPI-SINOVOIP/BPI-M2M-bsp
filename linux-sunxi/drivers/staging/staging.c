/*
 * drivers/staging/staging.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init staging_init(void)
{
	return 0;
}

static void __exit staging_exit(void)
{
}

module_init(staging_init);
module_exit(staging_exit);

MODULE_AUTHOR("Greg Kroah-Hartman");
MODULE_DESCRIPTION("Staging Core");
MODULE_LICENSE("GPL");
