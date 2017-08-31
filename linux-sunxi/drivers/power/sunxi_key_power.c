/*
 * drivers/power/sunxi_key_power.c
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
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/ioport.h>
#include <mach/sys_config.h>

struct key_pwr_data {
	s32 major;
	u32 dect_pin;
	u32 gpio_irq_num;
	u32 key_val;
	s32 ev_press;
	u32 hold_time;
	struct cdev key_pwr_cdev;
	struct class *cls;
	struct fasync_struct *async;
	struct timer_list timer;
};

u32 sun8i_pwr_pin;
static DECLARE_WAIT_QUEUE_HEAD(pwr_key_waitq);
static struct key_pwr_data key_pwr_dev = {
	.hold_time = 0,
};

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	mod_timer(&key_pwr_dev.timer, jiffies + HZ / 10);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function(unsigned long data)
{
	u32 pinval;
	static u32 key_pressed_time;
	pinval = __gpio_get_value(key_pwr_dev.dect_pin);

	if (pinval) {
		key_pwr_dev.key_val = 1;
		++key_pressed_time;
		mod_timer(&key_pwr_dev.timer, jiffies + HZ / 10);
	} else {
		/* ËÉ¿ª */
		key_pwr_dev.key_val = 0;
		key_pressed_time = 0;
	}

	if ((key_pressed_time / 10) >= key_pwr_dev.hold_time) {
		key_pwr_dev.ev_press = 1;
		key_pressed_time = 0;
		wake_up_interruptible(&pwr_key_waitq);
		kill_fasync(&key_pwr_dev.async, SIGIO, POLL_IN);
		del_timer(&key_pwr_dev.timer);
	}
}

int key_pwr_fasync(int fd, struct file *file, int on)
{
	return fasync_helper(fd, file, on, &key_pwr_dev.async);
}

unsigned int key_pwr_poll(struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &pwr_key_waitq, wait);

	if (key_pwr_dev.ev_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;

}

ssize_t key_pwr_read(struct file *file, char __user *buf, size_t size,
		     loff_t *ppos)
{
	if (size != 1)
		return -EINVAL;

	if (file->f_flags & O_NONBLOCK) {
		if (!key_pwr_dev.ev_press)
			return -EAGAIN;
	} else {
		wait_event_interruptible(pwr_key_waitq, key_pwr_dev.ev_press);
	}

	copy_to_user(buf, &key_pwr_dev.key_val, 1);
	key_pwr_dev.ev_press = 0;

	return 1;

}

int key_pwr_open(struct inode *inoede, struct file *file)
{
	init_timer(&key_pwr_dev.timer);
	key_pwr_dev.timer.function = buttons_timer_function;
	key_pwr_dev.gpio_irq_num = gpio_to_irq(key_pwr_dev.dect_pin);
	request_irq(key_pwr_dev.gpio_irq_num, buttons_irq, IRQF_TRIGGER_RISING,
		    "pwr_key", NULL);

	return 0;
}

int key_pwr_close(struct inode *inode, struct file *file)
{
	free_irq(key_pwr_dev.gpio_irq_num, NULL);
	del_timer(&key_pwr_dev.timer);

	return 0;
}

static const struct file_operations key_pwr_fops = {
	.owner = THIS_MODULE,
	.open = key_pwr_open,
	.read = key_pwr_read,
	.release = key_pwr_close,
	.poll = key_pwr_poll,
	.fasync = key_pwr_fasync,
};

static int sunxi_pwr_key_fetch_sysconfig_para(void)
{
	script_item_u val;
	script_item_value_type_e type;

	type = script_get_item("pwr_ctrl", "pwr_ctrl_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		printk(KERN_ERR
		       "%s script_parser_fetch \"pwr_ctrl\" pwr_ctrl_used = %d\n",
		       __func__, val.val);
		goto script_get_err;
	}

	if (!val.val) {
		printk(KERN_ERR "%s pwr_ctrl is not used in config\n",
		       __func__);
		goto script_get_err;
	}
	type = script_get_item("pwr_ctrl", "dect_pin", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "get dect_pin failed!\n");
		goto script_get_err;
	} else {
		key_pwr_dev.dect_pin = val.gpio.gpio;
		type = script_get_item("pwr_ctrl", "hold_time", &val);
		if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
			printk(KERN_ERR "get hold_time failed!");
			goto script_get_err;
		} else
			key_pwr_dev.hold_time = val.val;
	}
	type = script_get_item("pwr_ctrl", "pwr_pin", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type)
		printk(KERN_ERR "get pwr_pin failed!\n");
	sun8i_pwr_pin = val.gpio.gpio;
	return 0;
script_get_err:
	pr_notice("=========script_get_err============\n");
	return -1;
}

EXPORT_SYMBOL(sun8i_pwr_pin);
static int __init key_pwr_init(void)
{
	dev_t devid;
	int ret = 0;

	ret = sunxi_pwr_key_fetch_sysconfig_para();
	if (ret)
		goto err_exit;
	ret = alloc_chrdev_region(&devid, 0, 1, "key_pwr");
	if (ret)
		goto err_exit;
	key_pwr_dev.major = MAJOR(devid);
	cdev_init(&key_pwr_dev.key_pwr_cdev, &key_pwr_fops);
	ret = cdev_add(&key_pwr_dev.key_pwr_cdev, devid, 1);
	if (ret < 0)
		goto err_cdev_failed;

	key_pwr_dev.cls = class_create(THIS_MODULE, "key_pwr");
	device_create(key_pwr_dev.cls, NULL, MKDEV(key_pwr_dev.major, 0), NULL,
		      "key_pwr");

	return 0;
err_cdev_failed:
	unregister_chrdev_region(MKDEV(key_pwr_dev.major, 0), 1);
err_exit:
	return ret;
}

static void __exit key_pwr_exit(void)
{
	device_destroy(key_pwr_dev.cls, MKDEV(key_pwr_dev.major, 0));
	class_destroy(key_pwr_dev.cls);

	cdev_del(&key_pwr_dev.key_pwr_cdev);
	unregister_chrdev_region(MKDEV(key_pwr_dev.major, 0), 1);
}

module_init(key_pwr_init);
module_exit(key_pwr_exit);
MODULE_AUTHOR("Weng Zhilong");
MODULE_LICENSE("GPL");
