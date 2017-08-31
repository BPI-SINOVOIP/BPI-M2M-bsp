/*
 * drivers/misc/lradc_dev.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

static struct resource lradc_resource[] = {
[0] = {
	.start = 0x01c22800,
	.end   = 0x01c22800 + 16*4 - 1,
	.flags = IORESOURCE_MEM,
	},
[1] = {
	.start = GPIOB(4),
	.end   = GPIOB(4),
	.flags = IORESOURCE_IRQ,
}
};

static void lradc_release(struct device *dev)
{
}


static struct platform_device lradc_dev = {
.name         = "lradc_battery",
.id       = -1,
.num_resources    = ARRAY_SIZE(lradc_resource),
.resource     = lradc_resource,
.dev = {
	.release = lradc_release,
	},
};

static int lradc_dev_init(void)
{
	platform_device_register(&lradc_dev);
	return 0;
}

static void lradc_dev_exit(void)
{
	platform_device_unregister(&lradc_dev);
}

module_init(lradc_dev_init);
module_exit(lradc_dev_exit);

MODULE_LICENSE("GPL");

