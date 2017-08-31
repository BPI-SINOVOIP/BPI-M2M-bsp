/*
 * lib/dump_stack.c
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
 * Provide a default dump_stack() function for architectures
 * which don't implement their own.
 */

#include <linux/kernel.h>
#include <linux/export.h>

void dump_stack(void)
{
	printk(KERN_NOTICE
		"This architecture does not implement dump_stack()\n");
}

EXPORT_SYMBOL(dump_stack);
