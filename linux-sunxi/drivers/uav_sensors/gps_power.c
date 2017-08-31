/*
 * Copyright (C) 2013 Allwinnertech
 * Heming <lvheming@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <mach/gpio.h>
#include <mach/sys_config.h>
#include <linux/gpio.h>
#include <linux/pinctrl/pinconf-sunxi.h>

static u32 gps_power_gpio;

static int gps_power_val;
static struct kobject *gps_power_kobj;

extern int pin_config_set(const char *dev_name, const char *name,
			  unsigned long config);
static int gps_power_request_gpio(struct gpio_config *pinConfig,
				  __u32 group_count_max)
{
	struct gpio_config pin_cfg;
	char pin_name[32];
	int ret;
	u32 config;

	pin_cfg.gpio = pinConfig->gpio;
	pin_cfg.mul_sel = pinConfig->mul_sel;
	pin_cfg.pull = pinConfig->pull;
	pin_cfg.drv_level = pinConfig->drv_level;
	pin_cfg.data = pinConfig->data;


	ret = gpio_request(pin_cfg.gpio, NULL);
	if (0 == ret) {
		sunxi_gpio_to_name(pin_cfg.gpio, pin_name);
		config =
		    SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg.mul_sel);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (pin_cfg.pull != GPIO_PULL_DEFAULT) {
			config =
			    SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD,
					      pin_cfg.pull);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.drv_level != GPIO_DRVLVL_DEFAULT) {
			config =
			    SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DRV,
					      pin_cfg.drv_level);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.data != GPIO_DATA_DEFAULT) {

			config =
			    SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT,
					      pin_cfg.data);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
	}

	return ret;
}

static int gps_power_set_gpio_val(u32 gpio, u32 val)
{
	struct gpio_config pin_cfg;
	int ret = -1;

	pin_cfg.gpio = gpio;
	pin_cfg.drv_level = 1;
	pin_cfg.mul_sel = 1;
	pin_cfg.data = ((val) ? 1 : 0);
	pin_cfg.pull = 1;

	ret = gps_power_request_gpio(&pin_cfg, 0);
	if (0 == ret)
		gpio_free(gpio);

	return ret;
}

static ssize_t gps_power_show(struct kobject *kobj, struct kobj_attribute *attr,
			      char *buf)
{
	return sprintf(buf, "gps_power:%d\r\n", gps_power_val);
}

static ssize_t gps_power_store(struct kobject *kobj,
			       struct kobj_attribute *attr, const char *buf,
			       size_t count)
{
	if (buf[0] == '1') {
		gps_power_val = 1;
		gps_power_set_gpio_val(gps_power_gpio, gps_power_val);
	} else if (buf[0] == '0') {
		gps_power_val = 0;
		gps_power_set_gpio_val(gps_power_gpio, gps_power_val);
	}


	return count;
}

static struct kobj_attribute gps_power_attr =
__ATTR(gps_power, 0777, gps_power_show, gps_power_store);

static struct attribute *gps_power_attrs[] = {
	&gps_power_attr.attr,
	NULL,/* need to NULL terminate the list of attributes */
};

static struct attribute_group gps_power_attr_group = {
	.attrs = gps_power_attrs,
};

static int __init gps_power_init(void)
{
	int retval;
	int data;
	script_item_u val;
	script_item_value_type_e type;

	type = script_get_item("gps_power_para", "gps_power_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("%s: type err device_used =%d.\n",
		__func__, val.val);
		return -1;
	}
	data = val.val;

	printk(KERN_INFO "gps_power_init used:%d \r\n", data);

	type = script_get_item("gps_power_para", "gps_power_pin", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "failed to fetch gps_power power pin\n");
		return -1;
	}
	gps_power_gpio = val.gpio.gpio;

	gps_power_kobj = kobject_create_and_add("gps_power", NULL);
	if (!gps_power_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(gps_power_kobj, &gps_power_attr_group);
	if (retval)
		kobject_put(gps_power_kobj);

	return retval;
}

static void __exit gps_power_exit(void)
{
	kobject_put(gps_power_kobj);
}

module_init(gps_power_init);
module_exit(gps_power_exit);

MODULE_DESCRIPTION("sunxi gps power driver");
MODULE_AUTHOR("lvyuanzhou");
MODULE_LICENSE("GPL");
