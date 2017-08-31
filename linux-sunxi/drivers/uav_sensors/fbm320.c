/*
 * drivers/uav_sensors/fbm320.c
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

#define SENSOR_NAME "fbm320"
#define SENSOR_I2C_SLAVE_ADDRESS 0x6C

static struct i2c_client *fbm320_i2c_client;

typedef struct {
	uint16_t C0, C1, C2, C3, C6, C8, C9, C10, C11, C12;
	uint32_t C4, C5, C7;
} fbm320_calib_data;

struct fbm320_report_s {
	uint8_t mode;
	int32_t pressure;
	int32_t temp;
	fbm320_calib_data calib;
};

static fbm320_calib_data fbm320_calib;

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static dev_t devno;
static struct cdev *cdev;
static struct device *device;
static struct class *cls;

int fbm320_read8(struct i2c_client *client, uint8_t reg, uint8_t *data)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, reg, 1, data);

	return ret;
}

int fbm320_read32_data(struct i2c_client *client, uint32_t *data)
{
	uint8_t value[3];
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, 0xF6, 3, value);
	*data =
	    ((uint32_t) value[0] << 16) | ((uint32_t) value[1] << 8) |
	    (uint32_t) value[2];

	return ret;
}

int fbm320_write8(struct i2c_client *client, uint8_t reg, uint8_t val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, val);

	return ret;
}

static bool fbm320_i2c_test(struct i2c_client *client)
{
	uint8_t id;
	int ret;

	ret = fbm320_read8(client, 0x6B, &id);
	if (ret < 0) {
		printk(KERN_INFO "==%s== get id failed\r\n", __func__);
		return false;
	}

	printk(KERN_INFO "==%s== %x\r\n", __func__, id);

	return true;
}

static int fbm320_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

int fbm320_readCoefficients(struct i2c_client *client)
{
	int ret;
	uint8_t i;
	uint8_t tmp[2];
	uint16_t R[10] = { 0 };

	for (i = 0; i < 9; i++) {
		ret = fbm320_read8(client, 0xAA + (i * 2), &tmp[0]);
		if (ret < 0)
			return -1;
		ret = fbm320_read8(client, 0xAB + (i * 2), &tmp[1]);
		if (ret < 0)
			return -1;
		R[i] = ((uint8_t) tmp[0] << 8 | tmp[1]);
	}

	ret = fbm320_read8(client, 0xA4, &tmp[0]);
	if (ret < 0)
		return -1;
	ret = fbm320_read8(client, 0xF1, &tmp[0]);
	if (ret < 0)
		return -1;
	R[9] = ((uint8_t) tmp[0] << 8) | tmp[1];

	/*      Use R0~R9 calculate C0~C12 of FBM320-02 */
	fbm320_calib.C0 = R[0] >> 4;
	fbm320_calib.C1 = ((R[1] & 0xFF00) >> 5) | (R[2] & 7);
	fbm320_calib.C2 = ((R[1] & 0xFF) << 1) | (R[4] & 1);
	fbm320_calib.C3 = R[2] >> 3;
	fbm320_calib.C4 = ((uint32_t) R[3] << 2) | (R[0] & 3);
	fbm320_calib.C5 = R[4] >> 1;
	fbm320_calib.C6 = R[5] >> 3;
	fbm320_calib.C7 = ((uint32_t) R[6] << 3) | (R[5] & 7);
	fbm320_calib.C8 = R[7] >> 3;
	fbm320_calib.C9 = R[8] >> 2;
	fbm320_calib.C10 = ((R[9] & 0xFF00) >> 6) | (R[8] & 3);
	fbm320_calib.C11 = R[9] & 0xFF;
	fbm320_calib.C12 = ((R[0] & 0x0C) << 1) | (R[7] & 7);

#if 0
	printk(KERN_INFO "C0:%x(%d) \r\n", fbm320_calib.C0, fbm320_calib.C0);
	printk(KERN_INFO "C1:%x(%d) \r\n", fbm320_calib.C1, fbm320_calib.C1);
	printk(KERN_INFO "C2:%x(%d) \r\n", fbm320_calib.C2, fbm320_calib.C2);
	printk(KERN_INFO "C3:%x(%d) \r\n", fbm320_calib.C3, fbm320_calib.C3);
	printk(KERN_INFO "C4:%x(%d) \r\n", fbm320_calib.C4, fbm320_calib.C4);
	printk(KERN_INFO "C5:%x(%d) \r\n", fbm320_calib.C5, fbm320_calib.C5);
	printk(KERN_INFO "C6:%x(%d) \r\n", fbm320_calib.C6, fbm320_calib.C6);
	printk(KERN_INFO "C7:%x(%d) \r\n", fbm320_calib.C7, fbm320_calib.C7);
	printk(KERN_INFO "C8:%x(%d) \r\n", fbm320_calib.C8, fbm320_calib.C8);
	printk(KERN_INFO "C9:%x(%d) \r\n", fbm320_calib.C9, fbm320_calib.C9);
	printk(KERN_INFO "C10:%x(%d) \r\n", fbm320_calib.C10, fbm320_calib.C10);
	printk(KERN_INFO "C11:%x(%d) \r\n", fbm320_calib.C11, fbm320_calib.C11);
	printk(KERN_INFO "C12:%x(%d) \r\n", fbm320_calib.C12, fbm320_calib.C12);
#endif

	return 0;
}

static int fbm320_init_client(struct i2c_client *client)
{
	int ret;
	ret = fbm320_readCoefficients(client);
	if (ret < 0)
		return -1;

	ret = fbm320_write8(client, 0xF4, 0x2E);
	if (ret < 0)
		return -1;

	printk(KERN_INFO "==%s== done\r\n", __func__);

	return 0;
}

static int fbm320_measure(struct i2c_client *client, struct fbm320_report_s *pf)
{
	int ret;

	pf->calib.C0 = fbm320_calib.C0;
	pf->calib.C1 = fbm320_calib.C1;
	pf->calib.C2 = fbm320_calib.C2;
	pf->calib.C3 = fbm320_calib.C3;
	pf->calib.C4 = fbm320_calib.C4;
	pf->calib.C5 = fbm320_calib.C5;
	pf->calib.C6 = fbm320_calib.C6;
	pf->calib.C7 = fbm320_calib.C7;
	pf->calib.C8 = fbm320_calib.C8;
	pf->calib.C9 = fbm320_calib.C9;
	pf->calib.C10 = fbm320_calib.C10;
	pf->calib.C11 = fbm320_calib.C11;
	pf->calib.C12 = fbm320_calib.C12;

	if (pf->mode) {
		ret = fbm320_read32_data(client, &pf->pressure);
		if (ret < 0)
			return -1;

		ret = fbm320_write8(client, 0xF4, 0x2E);
		if (ret < 0)
			return -1;
	} else {
		ret = fbm320_read32_data(client, &pf->temp);
		if (ret < 0)
			return -1;

		ret = fbm320_write8(client, 0xF4, 0xF4);
		if (ret < 0)
			return -1;
	}

	return 0;
}

int fbm320_open(struct inode *inode, struct file *filp)
{
	int ret;

	printk(KERN_INFO "==%s==\r\n", __func__);

	if (fbm320_i2c_test(fbm320_i2c_client) == false)
		return -1;

	ret = fbm320_init_client(fbm320_i2c_client);
	if (ret < 0)
		return -1;

	return 0;
}

int fbm320_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t fbm320_read(struct file *filp, char __user *buf, size_t size,
			   loff_t *ppos)
{
	int ret;
	struct fbm320_report_s data;

	copy_from_user((void *)&data, buf, sizeof(data));

	ret = fbm320_measure(fbm320_i2c_client, &data);
	if (ret < 0)
		return -1;

	copy_to_user(buf, (void *)&data, sizeof(data));

	return 1;
}

static const struct file_operations fbm320_fops = {
	.owner = THIS_MODULE,
	.read = fbm320_read,
	.open = fbm320_open,
	.release = fbm320_release,
};

static int fbm320_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (fbm320_i2c_client == NULL)
		fbm320_i2c_client = client;

	if (fbm320_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int fbm320_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id fbm320_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, fbm320_id);

static struct i2c_driver fbm320_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = fbm320_probe,
	.remove = fbm320_remove,
	.id_table = fbm320_id,
	.detect = fbm320_detect,
	.address_list = normal_i2c,
};

static int __init fbm320_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&fbm320_driver);
	if (ret < 0) {
		printk(KERN_INFO "add fbm320 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "fbm320");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &fbm320_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "fbm320");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n",
		__func__, ret);

	}

	device = device_create(cls, NULL, devno, NULL, "fbm320");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n",
		__func__, ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit fbm320_exit(void)
{
	printk(KERN_INFO "remove fbm320 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&fbm320_driver);
}

module_init(fbm320_init);
module_exit(fbm320_exit);

MODULE_DESCRIPTION("fbm320 3-Axis Orientation/Motion Detection Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
