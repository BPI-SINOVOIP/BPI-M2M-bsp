/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*
* Copyright (c) 2014
*
* ChangeLog
*
*
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/platform_device.h>

#include <linux/gpio.h>


#if defined(CONFIG_PM)
#include <linux/pm.h>
#endif

#include "lradc_battery.h"


struct lradc_bat_data {
	u32 voltage;
	struct platform_device *pdev;
	void __iomem *reg_base;
	int charging_gpio;
	int charging_irq;
	int is_charging;
};

static struct lradc_bat_data *battery_data_p;

static u32 lradc_read(void __iomem *reg_offset)
{
	u32 reg_val = 0;

	reg_val = readl(reg_offset);

	return reg_val;
}

static void lradc_write(void __iomem *reg_base, u32 val)
{
	writel(val, reg_base);
}

static ssize_t charging_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	return sprintf(buf, "%d\n",
		gpio_get_value(battery_data_p->charging_gpio));
}

static ssize_t adc_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	u32 data = 0;
	data = lradc_read(battery_data_p->reg_base + LRADC_DATA0);
	return sprintf(buf, "%u\n", data);
}

static struct device_attribute charging_attr = __ATTR_RO(charging);

static struct device_attribute adc_attr = __ATTR_RO(adc);

static int lradc_battery_probe(struct platform_device *pdev)
{
	int ret = 0;
	u32 val = 0;
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	battery_data_p = kzalloc(sizeof(*battery_data_p), GFP_KERNEL);
	if (IS_ERR_OR_NULL(battery_data_p)) {
		pr_err("battery_data: not enough memory for battery data\n");
		ret = -ENOMEM;
	}

	battery_data_p->reg_base =
	    ioremap(res->start, res->end - res->start + 1);
	if (NULL == battery_data_p->reg_base) {
		pr_err("%s:Failed to ioremap() io memory region.\n", __func__);
		ret = -EBUSY;
		goto failed;
	}


	/* initalize hardware */
	/* initalize interrutp control regitster */
	val = 0;
	lradc_write(battery_data_p->reg_base + LRADC_INTC, val);

	/* initalize control regitster */
	val = ADC_CHAN_SELECT | LRADC_SAMPLE_32HZ | LRADC_EN | LRADC_HOLD_EN;
	lradc_write(battery_data_p->reg_base + LRADC_CTRL, val);

	device_create_file(&pdev->dev, &adc_attr);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	battery_data_p->charging_gpio = res->start;
	if (battery_data_p->charging_gpio >= 0) {
		gpio_request(battery_data_p->charging_gpio, NULL);
		gpio_direction_input(battery_data_p->charging_gpio);
		device_create_file(&pdev->dev, &charging_attr);
	}

	printk(KERN_INFO "\nlradc_battery_probe\n\n");
	return 0;

failed:
	kfree(battery_data_p);
	battery_data_p = NULL;

	return ret;
}

static int lradc_battery_remove(struct platform_device *pdev)
{
	printk(KERN_INFO "\nlradc_battery_remove\n\n");

	lradc_write(battery_data_p->reg_base + LRADC_CTRL, 0);
	lradc_write(battery_data_p->reg_base + LRADC_INTC, 0);
	iounmap(battery_data_p->reg_base);
	if (battery_data_p)
		kfree(battery_data_p);
	battery_data_p = NULL;

	return 0;
}

static struct platform_driver lradc_battery_driver = {
	.probe = lradc_battery_probe,
	.remove = lradc_battery_remove,
	.driver = {
		   .name = "lradc_battery",
		   .owner = THIS_MODULE,
		   },
};

module_platform_driver(lradc_battery_driver);

MODULE_AUTHOR("Weng Zhilong");
MODULE_DESCRIPTION("lradc-battery driver");
MODULE_LICENSE("GPL");
