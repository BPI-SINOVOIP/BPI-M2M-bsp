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

#define WORK_DELAY (5*1000)
#define POWER_ADC_SPMPLE_NUM 4
#define BAT_HIGHT_RESISITOR     150
#define BAT_LOW_RESISITOR       100

#define BATTERY_LEVEL0 0
#define BATTERY_LEVEL1 20
#define BATTERY_LEVEL2 40
#define BATTERY_LEVEL3 60
#define BATTERY_LEVEL4 80
#define BATTERY_LEVEL5 100

struct lradc_bat_data {
	u32 voltage;
	u32 capacity;
	struct platform_device *pdev;
	struct delayed_work work;
	void __iomem *reg_base;
	int charging_gpio;
	int charging_irq;
	int is_charging;
};

enum LRADC_CHARGE_TYPE {
	LRADC_CHARGE_AC = 1,
	LRADC_CHARGE_USB_20,
	LRADC_CHARGE_USB_30,
};

static u32 g_usb_flag = LRADC_CHARGE_AC;

static struct lradc_bat_data *battery_data_p;

void lradc_set_usb_type(u32 type)
{
	g_usb_flag = type;
}
EXPORT_SYMBOL_GPL(lradc_set_usb_type);

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

static ssize_t voltage_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%umv\n", battery_data_p->voltage);
}

static ssize_t capacity_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", battery_data_p->capacity);
}

static ssize_t charging_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n",
			(gpio_get_value(battery_data_p->charging_gpio)
			? g_usb_flag : 0));
}

static ssize_t adc_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	u32 data = 0;
	memset(buf, 0, PAGE_SIZE);

	data = lradc_read(battery_data_p->reg_base + LRADC_DATA1);
	return snprintf(buf, PAGE_SIZE, "%u\n", data);
}

static struct device_attribute voltage_attr = __ATTR_RO(voltage);

static struct device_attribute charging_attr = __ATTR_RO(charging);

static struct device_attribute adc_attr = __ATTR_RO(adc);

static struct device_attribute capacity_attr = __ATTR_RO(capacity);

/*
static irqreturn_t lradc_bat_isr_key(int irq, void *dummy)
{
	u32 val = 0;
	u32 data = 0;


	val = lradc_read(battery_data_p->reg_base + LRADC_REG_INT_STA);
	data = lradc_read(battery_data_p->reg_base + LRADC_REG_DATA0);


	printk("lradc_bat_isr_key--> init status:0x%08x data:0x%08x\n",
		val, data);

	lradc_write(battery_data_p->reg_base + LRADC_REG_INT_STA, val);
	return IRQ_HANDLED;
}
*/
static u32 power_get_average(u32 *val, u32 num)
{
	u32 total;
	u32 i;

	if (num <= 0)
		return 0;


	total = 0;
	for (i = 0; i < num; i++)
		total += val[i];


	return total / num;
}

static u32 calc_voltage(u32 adc_val)
{
	return adc_val * 2000 / 63 * (BAT_HIGHT_RESISITOR +
		BAT_LOW_RESISITOR) / BAT_LOW_RESISITOR;
}

static u32 calc_capacity(u32 voltage)
{
	u32 cap = 0;
	if (voltage < 3500)
		cap = BATTERY_LEVEL0;
	else if (voltage <= 3650)
		cap = BATTERY_LEVEL1;
	else if (voltage <= 3750)
	      cap = BATTERY_LEVEL2;
	else if (voltage <= 3850)
	      cap = BATTERY_LEVEL3;
	else if (voltage <= 3950)
	      cap = BATTERY_LEVEL4;
	else if (voltage <= 4200)
	      cap = BATTERY_LEVEL5;
	else
	      cap = BATTERY_LEVEL5;

	return cap;
}

static void process_adc_val(struct lradc_bat_data *bat_data, u32 val)
{
	static u32 cur_adc_sample_num;
	static u32 adc_val[POWER_ADC_SPMPLE_NUM] = { 0 };
	u32 tmp_data = 0;
	int i = 0;

	if (cur_adc_sample_num < POWER_ADC_SPMPLE_NUM)
		adc_val[cur_adc_sample_num++] = val;
	else {
		for (i = 0; i < POWER_ADC_SPMPLE_NUM - 1; i++)
			adc_val[i] = adc_val[i + 1];
		adc_val[POWER_ADC_SPMPLE_NUM - 1] = val;
	}

	tmp_data = power_get_average(adc_val, cur_adc_sample_num);
	bat_data->voltage = calc_voltage(tmp_data);
	bat_data->capacity = calc_capacity(bat_data->voltage);
}

static void bat_work_func(struct work_struct *work)
{
	struct lradc_bat_data *bat_data =
	    container_of((struct delayed_work *)work,
			 struct lradc_bat_data, work);
	u32 data = 0;

	data = lradc_read(bat_data->reg_base + LRADC_DATA1);
	process_adc_val(bat_data, data);
	schedule_delayed_work(&bat_data->work, msecs_to_jiffies(WORK_DELAY));
}

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

	INIT_DELAYED_WORK(&battery_data_p->work, bat_work_func);

	/* initalize hardware */
	/* initalize interrutp control regitster */
	val = 0;
	lradc_write(battery_data_p->reg_base + LRADC_INTC, val);

	/* initalize control regitster */
	val = ADC_CHAN_SELECT | LRADC_SAMPLE_32HZ | LRADC_EN | LRADC_HOLD_EN;
	lradc_write(battery_data_p->reg_base + LRADC_CTRL, val);

	device_create_file(&pdev->dev, &voltage_attr);
	device_create_file(&pdev->dev, &adc_attr);
	device_create_file(&pdev->dev, &capacity_attr);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	battery_data_p->charging_gpio = res->start;
	if (battery_data_p->charging_gpio >= 0) {
		gpio_request(battery_data_p->charging_gpio, NULL);
		gpio_direction_input(battery_data_p->charging_gpio);
		device_create_file(&pdev->dev, &charging_attr);
	}
	schedule_delayed_work(&battery_data_p->work,
				msecs_to_jiffies(WORK_DELAY));

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

	cancel_delayed_work_sync(&battery_data_p->work);
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
