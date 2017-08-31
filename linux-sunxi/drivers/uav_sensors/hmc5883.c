/*
 * drivers/uav_sensors/hmc5883.c
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

#define SENSOR_NAME "hmc5883"
#define SENSOR_I2C_SLAVE_ADDRESS 0x1E

#define HMC5883_ADDR_CONF_A				0x00
#define HMC5883_ADDR_CONF_B				0x01
#define HMC5883_ADDR_MODE				0x02
#define HMC5883_ADDR_DATA_OUT_X_MSB		0x03
#define HMC5883_ADDR_DATA_OUT_X_LSB		0x04
#define HMC5883_ADDR_DATA_OUT_Z_MSB		0x05
#define HMC5883_ADDR_DATA_OUT_Z_LSB		0x06
#define HMC5883_ADDR_DATA_OUT_Y_MSB		0x07
#define HMC5883_ADDR_DATA_OUT_Y_LSB		0x08
#define HMC5883_ADDR_STATUS				0x09
#define HMC5883_ADDR_ID_A				0x0a
#define HMC5883_ADDR_ID_B				0x0b
#define HMC5883_ADDR_ID_C				0x0c
/* temperature on hmc5983 only */
#define HMC5883_ADDR_TEMP_OUT_MSB		0x31
#define HMC5883_ADDR_TEMP_OUT_LSB		0x32

#define ID_A_WHO_AM_I			'H'
#define ID_B_WHO_AM_I			'4'
#define ID_C_WHO_AM_I			'3'

/* ConfigRegA valid sample averaging for 5883L */
#define SampleAveraging_1    0x00
#define SampleAveraging_2    0x01
#define SampleAveraging_4    0x02
#define SampleAveraging_8    0x03

/* ConfigRegA valid data output rates for 5883L */
#define DataOutputRate_0_75HZ 0x00
#define DataOutputRate_1_5HZ  0x01
#define DataOutputRate_3HZ    0x02
#define DataOutputRate_7_5HZ  0x03
#define DataOutputRate_15HZ   0x04
#define DataOutputRate_30HZ   0x05
#define DataOutputRate_75HZ   0x06

#define PositiveBiasConfig   0x11
#define NormalOperation      0x10
#define magGain              0x20
#define ContinuousConversion 0x00
#define SingleConversion     0x01

/* modes not changeable outside of driver */
#define HMC5883L_MODE_NORMAL		(0 << 0)	/* default */
#define HMC5883L_MODE_POSITIVE_BIAS	(1 << 0)	/* positive bias */
#define HMC5883L_MODE_NEGATIVE_BIAS	(1 << 1)	/* negative bias */

#define HMC5883L_AVERAGING_1		(0 << 5)	/* conf a register */
#define HMC5883L_AVERAGING_2		(1 << 5)
#define HMC5883L_AVERAGING_4		(2 << 5)
#define HMC5883L_AVERAGING_8		(3 << 5)

#define MODE_REG_CONTINOUS_MODE		(0 << 0)
#define MODE_REG_SINGLE_MODE		(1 << 0)	/* default */

#define STATUS_REG_DATA_OUT_LOCK	(1 << 1)
#define STATUS_REG_DATA_READY		(1 << 0)
#define HMC5983_TEMP_SENSOR_ENABLE	(1 << 7)

static struct i2c_client *hmc5883_i2c_client;

struct hmc5883_report_s {
	int16_t x;
	int16_t y;
	int16_t z;
};

int hmc5883_scale_init;
int hmc5883_scale_init_count;
uint8_t hmc5883_base_config;

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static dev_t devno;
static struct cdev *cdev;
static struct device *device;
static struct class *cls;

static int hmc5883_i2c_test(struct i2c_client *client)
{
	unsigned char data[3];

	data[0] = i2c_smbus_read_byte_data(client, HMC5883_ADDR_ID_A);
	data[1] = i2c_smbus_read_byte_data(client, HMC5883_ADDR_ID_B);
	data[2] = i2c_smbus_read_byte_data(client, HMC5883_ADDR_ID_C);

	printk(KERN_INFO "ID=(%x,%x,%x)(%c,%c,%c)\r\n",
	data[0], data[1], data[2], data[0], data[1], data[2]);

	if (data[0] != ID_A_WHO_AM_I || data[1] != ID_B_WHO_AM_I
	    || data[2] != ID_C_WHO_AM_I) {
		return -1;
	}

	return 0;
}

static int hmc5883_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);

	return 0;
}

static int hmc5883_init_client(struct i2c_client *client)
{
	int ret;
	uint8_t calibration_gain = 0x60;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_CONF_A,
				      SampleAveraging_8 << 5 |
				      DataOutputRate_75HZ << 2 |
				      NormalOperation);
	if (ret < 0)
		return -1;
	ret =
	    i2c_smbus_read_i2c_block_data(client, HMC5883_ADDR_CONF_A, 1,
					  &hmc5883_base_config);
	if (ret < 0)
		return -1;

	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_CONF_A,
				      PositiveBiasConfig);
	if (ret < 0)
		return -1;
	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_CONF_B,
				      calibration_gain);
	if (ret < 0)
		return -1;
	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_MODE,
				      SingleConversion);
	if (ret < 0)
		return -1;

	return 0;
}

static int hmc5883_reinit_client(struct i2c_client *client)
{
	int ret;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_CONF_A,
				      hmc5883_base_config);
	if (ret < 0)
		return -1;
	ret = i2c_smbus_write_byte_data(client, HMC5883_ADDR_CONF_B, magGain);
	if (ret < 0)
		return -1;
	ret =
	    i2c_smbus_write_byte_data(client, HMC5883_ADDR_MODE,
				      ContinuousConversion);
	if (ret < 0)
		return -1;

	return 0;
}

static int hmc5883_measure(struct i2c_client *client,
			   struct hmc5883_report_s *pf)
{
	int ret;
	char buf[6];

	ret =
	    i2c_smbus_read_i2c_block_data(client, HMC5883_ADDR_DATA_OUT_X_MSB,
					  6, buf);
	if (ret < 0)
		return -1;

	pf->x = (((int16_t) buf[0]) << 8) | buf[1];
	pf->z = (((int16_t) buf[2]) << 8) | buf[3];
	pf->y = (((int16_t) buf[4]) << 8) | buf[5];

	if (pf->x == -4096 || pf->y == -4096 || pf->y == -4096) {
		pf->x = 0;
		pf->y = 0;
		pf->z = 0;
	}

	pf->x = -pf->x;
	pf->y = pf->y;
	pf->z = -pf->z;

	return 0;
}

int hmc5883_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);

	if (hmc5883_i2c_test(hmc5883_i2c_client) < 0)
		return -1;

	if (hmc5883_init_client(hmc5883_i2c_client) < 0)
		return -1;

	hmc5883_scale_init = 0;
	hmc5883_scale_init_count = 0;

	return 0;
}

int hmc5883_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t hmc5883_read(struct file *filp, char __user *buf, size_t size,
			    loff_t *ppos)
{
	struct hmc5883_report_s data;
	int ret;

	if (!hmc5883_scale_init) {
		hmc5883_scale_init_count++;
		if (hmc5883_scale_init_count >= 30) {
			ret = hmc5883_reinit_client(hmc5883_i2c_client);
			if (ret < 0)
				return -1;
			else
				hmc5883_scale_init = 1;
		}
	}

	ret = hmc5883_measure(hmc5883_i2c_client, &data);
	if (ret < 0)
		return -1;

	copy_to_user(buf, (void *)&data, sizeof(data));

	return 1;
}

static const struct file_operations hmc5883_fops = {
	.owner = THIS_MODULE,
	.read = hmc5883_read,
	.open = hmc5883_open,
	.release = hmc5883_release,
};

static int hmc5883_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	printk(KERN_INFO"==%s==\r\n", __func__);
	if (hmc5883_i2c_client == NULL)
		hmc5883_i2c_client = client;

	if (hmc5883_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int hmc5883_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id hmc5883_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, hmc5883_id);

static struct i2c_driver hmc5883_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = hmc5883_probe,
	.remove = hmc5883_remove,
	.id_table = hmc5883_id,
	.detect = hmc5883_detect,
	.address_list = normal_i2c,
};

static int __init hmc5883_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&hmc5883_driver);
	if (ret < 0) {
		printk(KERN_INFO "add hmc5883 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "hmc5883");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &hmc5883_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "hmc5883");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n",
		__func__, ret);

	}

	device = device_create(cls, NULL, devno, NULL, "hmc5883");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n",
		__func__, ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit hmc5883_exit(void)
{
	printk(KERN_INFO "remove hmc5883 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&hmc5883_driver);
}

module_init(hmc5883_init);
module_exit(hmc5883_exit);

MODULE_DESCRIPTION("hmc5883 mag Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
