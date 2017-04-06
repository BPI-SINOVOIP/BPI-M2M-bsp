/*
 * LED Heartbeat Trigger
 *
 * Copyright (C) 2006 Atsushi Nemoto <anemo@mba.ocn.ne.jp>
 *
 * Based on Richard Purdie's ledtrig-timer.c and some arch's
 * CONFIG_HEARTBEAT code.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include "leds.h"

struct doubleflash_trig_data {
	unsigned int phase;
	unsigned int period;
	struct timer_list timer;
	unsigned long delay_on;
	unsigned long first_delay_off;
	unsigned long second_delay_off;
};

static void led_doubleflash_function(unsigned long data)
{
	struct led_classdev *led_cdev = (struct led_classdev *) data;
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;
	unsigned long brightness = LED_OFF;
	unsigned long delay = 0;
	unsigned long delay_off = 0;
	unsigned long delay_on = 0;

	switch (doubleflash_data->phase) {
	case 0:
	case 1:
		delay_on = msecs_to_jiffies(doubleflash_data->delay_on);
		delay_off = msecs_to_jiffies(doubleflash_data->first_delay_off);
		doubleflash_data->phase++;
		break;
	case 2:
	case 3:
		delay_on = msecs_to_jiffies(doubleflash_data->delay_on);
		delay_off = msecs_to_jiffies(doubleflash_data->second_delay_off);
		if (doubleflash_data->phase == 3)
			doubleflash_data->phase = 0;
		else
			doubleflash_data->phase++;
		break;
	default:
		break;
	}

	brightness = led_get_brightness(led_cdev);
	if (!brightness) {
		delay = delay_on;
		brightness = LED_FULL;
	} else {
		delay = delay_off;
		brightness = LED_OFF;
	}

	led_set_brightness(led_cdev, brightness);
	mod_timer(&doubleflash_data->timer, jiffies + delay);
}

static inline ssize_t led_delay_on_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;

	return sprintf(buf, "%lu\n", doubleflash_data->delay_on);
}

static inline ssize_t led_delay_on_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;

	if (isspace(*after))
		count++;

	if (count == size) {
		doubleflash_data->delay_on = state;
		ret = count;
	}

	return ret;
}

static inline ssize_t led_first_delay_off_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;

	return sprintf(buf, "%lu\n", doubleflash_data->first_delay_off);
}

static inline ssize_t led_first_delay_off_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		doubleflash_data->first_delay_off = state;
		ret = count;
	}

	return ret;
}

static inline ssize_t led_second_delay_off_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;

	return sprintf(buf, "%lu\n", doubleflash_data->second_delay_off);
}

static inline ssize_t led_second_delay_off_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		doubleflash_data->second_delay_off = state;
		ret = count;
	}

	return ret;
}


static DEVICE_ATTR(delay_on, 0644, led_delay_on_show, led_delay_on_store);
static DEVICE_ATTR(first_delay_off, 0644, led_first_delay_off_show, led_first_delay_off_store);
static DEVICE_ATTR(second_delay_off, 0644, led_second_delay_off_show, led_second_delay_off_store);


static void doubleflash_trig_activate(struct led_classdev *led_cdev)
{
	int rc;
	struct doubleflash_trig_data *doubleflash_data;

	rc = device_create_file(led_cdev->dev, &dev_attr_delay_on);
	if (rc)
		return;
	rc = device_create_file(led_cdev->dev, &dev_attr_first_delay_off);
	if (rc)
		goto err_out_delayon;
	rc = device_create_file(led_cdev->dev, &dev_attr_second_delay_off);
	if (rc)
		goto err_out_first_delayoff;

	doubleflash_data = kzalloc(sizeof(*doubleflash_data), GFP_KERNEL);
	if (!doubleflash_data)
		goto failed;

	/*set defauly cycle time*/
	doubleflash_data->delay_on = 200;
	doubleflash_data->first_delay_off = 200;
	doubleflash_data->second_delay_off = 600;

	led_cdev->trigger_data = doubleflash_data;
	setup_timer(&doubleflash_data->timer,
		    led_doubleflash_function, (unsigned long) led_cdev);
	doubleflash_data->phase = 0;
	led_doubleflash_function(doubleflash_data->timer.data);

	return;

failed:
	device_remove_file(led_cdev->dev, &dev_attr_second_delay_off);
err_out_first_delayoff:
	device_remove_file(led_cdev->dev, &dev_attr_first_delay_off);
err_out_delayon:
	device_remove_file(led_cdev->dev, &dev_attr_delay_on);
}

static void doubleflash_trig_deactivate(struct led_classdev *led_cdev)
{
	struct doubleflash_trig_data *doubleflash_data = led_cdev->trigger_data;

	if (doubleflash_data) {
		del_timer_sync(&doubleflash_data->timer);
		device_remove_file(led_cdev->dev, &dev_attr_delay_on);
		device_remove_file(led_cdev->dev, &dev_attr_first_delay_off);
		device_remove_file(led_cdev->dev, &dev_attr_second_delay_off);
		kfree(doubleflash_data);
	}
}

static struct led_trigger doubleflash_led_trigger = {
	.name     = "doubleflash",
	.activate = doubleflash_trig_activate,
	.deactivate = doubleflash_trig_deactivate,
};

static int __init doubleflash_trig_init(void)
{
	return led_trigger_register(&doubleflash_led_trigger);
}

static void __exit doubleflash_trig_exit(void)
{
	led_trigger_unregister(&doubleflash_led_trigger);
}

module_init(doubleflash_trig_init);
module_exit(doubleflash_trig_exit);

MODULE_AUTHOR("Atsushi Nemoto <anemo@mba.ocn.ne.jp>");
MODULE_DESCRIPTION("Doubleflash LED trigger");
MODULE_LICENSE("GPL");
