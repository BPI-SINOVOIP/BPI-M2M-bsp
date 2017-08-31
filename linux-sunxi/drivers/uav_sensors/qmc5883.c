/*
 * drivers/uav_sensors/qmc5883.c
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

#define SENSOR_NAME "qmc5883"
#define SENSOR_I2C_SLAVE_ADDRESS 0x0D

#define QMC5883_DATA_X_LSB_REG	0x00
#define QMC5883_CONTROL_REG		0x09
#define QMC5883_RESET_REG		0x0B
#define QMC5883_ID_REG			0x0D

#define QMC5883_ID_VAL	0xFF

static struct i2c_client *qmc5883_i2c_client;

struct qmc5883_report_s {
	int16_t x;
	int16_t y;
	int16_t z;
};

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static dev_t devno;
static struct cdev *cdev;
static struct device *device;
static struct class *cls;

static int qmc5883_i2c_test(struct i2c_client *client)
{
	unsigned char id;

	id = i2c_smbus_read_byte_data(client, QMC5883_ID_REG);
	printk(KERN_INFO "ID=%x(%d)\r\n", id, id);
	if (id != QMC5883_ID_VAL)
		return -1;


	return 0;
}

static int qmc5883_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);





	return 0;
}

static int qmc5883_init_client(struct i2c_client *client)
{
	uint8_t data = 0;

	i2c_smbus_write_byte_data(client, QMC5883_RESET_REG, 0x01);
	i2c_smbus_write_byte_data(client, 0x20, 0x40);
	i2c_smbus_write_byte_data(client, 0x21, 0x01);

	i2c_smbus_write_byte_data(client, QMC5883_CONTROL_REG, 0x0D);

	mdelay(5);

	while (data != 0x0D) {
		mdelay(2);
		data = i2c_smbus_read_byte_data(client, QMC5883_CONTROL_REG);
	}

	printk(KERN_INFO "==%s==\r\n", __func__);

	return 0;
}

static int qmc5883_measure(struct i2c_client *client,
			   struct qmc5883_report_s *pf)
{
	int ret = -1;
	char buf[6];

	ret =
	    i2c_smbus_read_i2c_block_data(client, QMC5883_DATA_X_LSB_REG, 6,
					  buf);
	if (ret < 0)
		return -1;
	else {
		pf->x = ((int16_t) buf[1] << 8) | buf[0];
		pf->y = ((int16_t) buf[3] << 8) | buf[2];
		pf->z = ((int16_t) buf[5] << 8) | buf[4];



		return 0;
	}
}

int qmc5883_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);

	if (qmc5883_i2c_test(qmc5883_i2c_client) < 0)
		return -1;


	if (qmc5883_init_client(qmc5883_i2c_client) < 0)
		return -1;


	return 0;
}

int qmc5883_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t qmc5883_read(struct file *filp, char __user *buf, size_t size,
			    loff_t *ppos)
{
	struct qmc5883_report_s data;
	int ret;

	ret = qmc5883_measure(qmc5883_i2c_client, &data);
	if (ret < 0)
		return -1;


	copy_to_user(buf, (void *)&data, sizeof(data));

	return 1;
}

static const struct file_operations qmc5883_fops = {
	.owner = THIS_MODULE,
	.read = qmc5883_read,
	.open = qmc5883_open,
	.release = qmc5883_release,
};

static int qmc5883_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (qmc5883_i2c_client == NULL)
		qmc5883_i2c_client = client;


	if (qmc5883_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);


	return 0;
}

static int qmc5883_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id qmc5883_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, qmc5883_id);

static struct i2c_driver qmc5883_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = qmc5883_probe,
	.remove = qmc5883_remove,
	.id_table = qmc5883_id,
	.detect = qmc5883_detect,
	.address_list = normal_i2c,
};

static int __init qmc5883_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&qmc5883_driver);
	if (ret < 0) {
		printk(KERN_INFO "add qmc5883 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "qmc5883");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &qmc5883_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "qmc5883");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n",
		__func__, ret);

	}

	device = device_create(cls, NULL, devno, NULL, "qmc5883");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n",
		__func__, ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit qmc5883_exit(void)
{
	printk(KERN_INFO "remove qmc5883 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&qmc5883_driver);
}

module_init(qmc5883_init);
module_exit(qmc5883_exit);

MODULE_DESCRIPTION("qmc5883 mag Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
