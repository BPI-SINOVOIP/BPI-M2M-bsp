/*
 * drivers/uav_sensors/ks103.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define SENSOR_NAME "ks103"
#define SENSOR_I2C_SLAVE_ADDRESS 0x74

#define KS103_REG 0x02

#define KS103_POWER0		0x70
#define KS103_POWER1		0x71
#define KS103_POWER2		0x72
#define KS103_POWER3		0x73
#define KS103_POWER4		0x74
#define KS103_POWER5		0x75

#define KS103_RANGE			0xB4

#define KS103_I2C_DOWN_OFF	0xC3

static struct i2c_client *ks103_i2c_client;

struct ks103_report_s {
	uint16_t distance;
};

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static dev_t devno;
static struct cdev *cdev;
static struct device *device;
static struct class *cls;

static bool ks103_init_client(struct i2c_client *client)
{
	int ret = -1;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_smbus_write_byte_data(client, KS103_REG, KS103_I2C_DOWN_OFF);
	if (ret < 0) {
		printk(KERN_INFO "==%s== KS103_I2C_DOWN_OFF failed\r\n",
				__func__);
		return false;
	}

	ret = i2c_smbus_write_byte_data(client, KS103_REG, KS103_POWER4);
	if (ret < 0) {
		printk(KERN_INFO "==%s== KS103_POWER2 failed\r\n", __func__);
		return false;
	}

	ret = i2c_smbus_write_byte_data(client, KS103_REG, KS103_RANGE);
	if (ret < 0) {
		printk(KERN_INFO "==%s== KS103_RANGE failed\r\n", __func__);
		return false;
	}

	printk(KERN_INFO "==%s== done\r\n", __func__);

	return true;
}

static int ks103_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

static int ks103_measure(struct i2c_client *client, struct ks103_report_s *pf)
{
	uint8_t buf[2];
	int result;

	result = i2c_smbus_read_i2c_block_data(client, KS103_REG, 2, buf);
	if (result < 0)
		return -1;

	result = i2c_smbus_write_byte_data(client, KS103_REG, KS103_RANGE);
	if (result < 0)
		return -1;

	pf->distance = (buf[0] << 8) + buf[1];

	return 0;
}

int ks103_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (ks103_init_client(ks103_i2c_client) == false)
		return -1;

	return 0;
}

int ks103_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t ks103_read(struct file *filp, char __user *buf, size_t size,
			  loff_t *ppos)
{
	struct ks103_report_s data;
	int result;

	result = ks103_measure(ks103_i2c_client, &data);
	if (result < 0)
		return -1;

	copy_to_user(buf, (void *)&data, sizeof(data));
	return 1;
}

static const struct file_operations ks103_fops = {
	.owner = THIS_MODULE,
	.read = ks103_read,
	.open = ks103_open,
	.release = ks103_release,
};

static int ks103_probe(struct i2c_client *client,
		       const struct i2c_device_id *id)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (ks103_i2c_client == NULL)
		ks103_i2c_client = client;

	if (ks103_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int ks103_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id ks103_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, ks103_id);

static struct i2c_driver ks103_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = ks103_probe,
	.remove = ks103_remove,
	.id_table = ks103_id,
	.detect = ks103_detect,
	.address_list = normal_i2c,
};

static int __init ks103_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&ks103_driver);
	if (ret < 0) {
		printk(KERN_INFO "add ks103 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "ks103");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &ks103_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "ks103");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n",
			__func__, ret);

	}

	device = device_create(cls, NULL, devno, NULL, "ks103");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n",
			__func__, ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit ks103_exit(void)
{
	printk(KERN_INFO "remove ks103 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&ks103_driver);
}

module_init(ks103_init);
module_exit(ks103_exit);

MODULE_DESCRIPTION("ks103 Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
