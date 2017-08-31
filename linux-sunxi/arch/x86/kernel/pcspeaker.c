/*
 * arch/x86/kernel/pcspeaker.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/init.h>

static __init int add_pcspkr(void)
{
	struct platform_device *pd;

	pd = platform_device_register_simple("pcspkr", -1, NULL, 0);

	return IS_ERR(pd) ? PTR_ERR(pd) : 0;
}
device_initcall(add_pcspkr);
