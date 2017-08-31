/*
 * drivers/leds/led-v3s.c
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/gpio.h>
#include <mach/sys_config.h>

#define LEDS_GPIO_ON  1
#define LEDS_GPIO_OFF 0
#define LED_DELAY_MSEC 500
struct gpio_led {
	const char *name;
	unsigned gpio;
	unsigned active_low:1;
	int default_state;
	int current_state;
	unsigned long interval;
	unsigned blink_count;
	struct timer_list blink_timer;
	/* default_state should be one of LEDS_GPIO_DEFSTATE_(ON|OFF|KEEP) */
};

static struct class *leds_class;
static struct device *leds_device;
static struct gpio_led gpio_leds = {
	.name = "default_led",
	.gpio = 0xffff,
	.blink_count = 0,
	.interval = LED_DELAY_MSEC,
};

static void gpio_led_switch(int state)
{
	gpio_set_value(gpio_leds.gpio, state);
	gpio_leds.current_state = state;
}

static void led_timer_function(unsigned long data)
{
	int led_state;
	static unsigned led_timer_count;

led_state = !gpio_leds.current_state;
gpio_led_switch(led_state);
mod_timer(&gpio_leds.blink_timer,
jiffies + msecs_to_jiffies(gpio_leds.interval));
if (gpio_leds.blink_count > 0) {

		++led_timer_count;

if (led_timer_count > gpio_leds.blink_count) {
	led_timer_count = 0;
if (led_state != gpio_leds.default_state)
	gpio_led_switch(gpio_leds.default_state);

del_timer_sync(&gpio_leds.blink_timer);
}

	}
}

static ssize_t led_switch_store(struct device *dev,
				struct device_attribute *attr, const char *buf,
				size_t size)
{
	ssize_t ret = -EINVAL;
	char *after;
	int state  = simple_strtoul(buf, &after,
		16) ? LEDS_GPIO_ON : LEDS_GPIO_OFF;
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		ret = count;
		gpio_led_switch(state);
	}

	return ret;
}

static ssize_t led_blink_store(struct device *dev,
			       struct device_attribute *attr, const char *buf,
			       size_t size)
{
	ssize_t ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 16);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		ret = count;
		if (state & 0x0f) {
			gpio_leds.blink_count = (state >> 4) & 0x0f;
			mod_timer(&gpio_leds.blink_timer, jiffies + 1);
		} else {
			del_timer_sync(&gpio_leds.blink_timer);
			gpio_led_switch(gpio_leds.default_state);
			gpio_leds.blink_count = 0;
		}
	}

	return ret;
}

static ssize_t led_def_state_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	char *after;
	int state  = simple_strtoul(buf, &after,
		16) ? LEDS_GPIO_ON : LEDS_GPIO_OFF;
	size_t count = after - buf;
	if (isspace(*after))
		count++;
	if (count == size) {
		ret = count;
		gpio_leds.default_state = state;
		gpio_led_switch(gpio_leds.default_state);
	}
	return ret;
}

static ssize_t led_def_state_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
return sprintf(buf, "%x\n", gpio_leds.default_state);
}


static ssize_t led_interval_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		if (state > 0)
			gpio_leds.interval = state & 0xffffffff;
		else
			gpio_leds.interval = LED_DELAY_MSEC;
	}

	return ret;
}

static struct device_attribute led_class_attrs[] = {
	__ATTR(switch, 0644, NULL, led_switch_store),
	__ATTR(blink, 0644, NULL, led_blink_store),
	__ATTR(interval, 0644, NULL, led_interval_store),
	__ATTR(default_state, 0666, led_def_state_show, led_def_state_store),
	__ATTR_NULL,
};

	static int sunxi_leds_fetch_sysconfig_para(void)
{
		int num = 0;
		script_item_u val;
		script_item_value_type_e type;
		static char *led_name[3] = { "led1", NULL };
		char led_active_low[25];

		type = script_get_item("led_para", "led_used", &val);
		if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
			printk(KERN_ERR
			       "%s script_parser_fetch \"leds_para\" leds_used = %d\n",
			       __func__, val.val);
			goto script_get_err;
		}

		if (!val.val) {
			printk(KERN_ERR "%s leds is not used in config\n",
			       __func__);
			goto script_get_err;
		}
		type = script_get_item("led_para", led_name[0], &val);
		if (SCIRPT_ITEM_VALUE_TYPE_PIO != type)
			printk(KERN_ERR "no %s, ignore it!\n", led_name[0]);
		else {
			gpio_leds.name = led_name[0];
			gpio_leds.gpio = val.gpio.gpio;
			gpio_leds.default_state =
			    val.gpio.data ? LEDS_GPIO_ON : LEDS_GPIO_OFF;
			sprintf(led_active_low, "%s_active_low", led_name[0]);
			type =
			    script_get_item("led_para", led_active_low, &val);
			if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
				printk(KERN_ERR "no %s, %s failed!\n",
				       led_active_low, __func__);
				goto script_get_err;
			} else
				gpio_leds.active_low = val.val ? true : false;
		}
		return num;
script_get_err:
		pr_notice("=========script_get_err============\n");
		return -1;
	}

	static int led_gpio_cfg(void)
{
		int ret = 0;
		ret = gpio_request(gpio_leds.gpio, gpio_leds.name);
		if (ret < 0)
			goto err_exit;
		ret = gpio_direction_output(gpio_leds.gpio,
			gpio_leds.default_state);
		if (ret < 0)
			goto err_exit;
		gpio_led_switch(gpio_leds.default_state);
err_exit:
		return ret;
	}

	static int __init leds_init(void)
{
		int ret = 0;
		ret = sunxi_leds_fetch_sysconfig_para();
		if (ret != -1) {
			ret = led_gpio_cfg();
			if (ret < 0)
				goto err_exit;

			init_timer(&gpio_leds.blink_timer);
			gpio_leds.blink_timer.function = led_timer_function;

			leds_class = class_create(THIS_MODULE, "leds");
			if (IS_ERR(leds_class)) {
				ret = PTR_ERR(leds_class);
				printk(KERN_ERR "class create failed!\n");
				goto err_exit;
			}
			leds_class->dev_attrs = led_class_attrs;

			leds_device = device_create(leds_class, NULL, 0, NULL,
						    "%s", gpio_leds.name);
		}
err_exit:
		return ret;
	}

	static void __exit leds_exit(void)
{
		gpio_free(gpio_leds.gpio);
		del_timer(&gpio_leds.blink_timer);
		device_unregister(leds_device);
		class_destroy(leds_class);
	}
	module_init(leds_init);
	module_exit(leds_exit);

	MODULE_AUTHOR("Weng Zhilong");
	MODULE_LICENSE("GPL");
	MODULE_DESCRIPTION("LED-V3s");
