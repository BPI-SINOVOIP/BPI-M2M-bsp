/*
 * drivers/uav_sensors/mpu6050.c
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

#define SENSOR_NAME "mpu6050"
#define SENSOR_I2C_SLAVE_ADDRESS	0x68

#define	MPU6050REG_SMPLRT_DIV		0x19
#define	MPU6050REG_CONFIG			0x1A
#define	MPU6050REG_GYRO_CONFIG		0x1B
#define	MPU6050REG_ACCEL_CONFIG		0x1C
#define	MPU6050REG_INT_PIN_CFG		0x37
#define	MPU6050REG_INT_ENABLE		0x38
#define MPU6050REG_INT_STATUS		0x3A
#define	MPU6050REG_ACCEL_XOUT_H		0x3B
#define	MPU6050REG_ACCEL_XOUT_L		0x3C
#define	MPU6050REG_ACCEL_YOUT_H		0x3D
#define	MPU6050REG_ACCEL_YOUT_L		0x3E
#define	MPU6050REG_ACCEL_ZOUT_H		0x3F
#define	MPU6050REG_ACCEL_ZOUT_L		0x40
#define	MPU6050REG_TEMP_OUT_H		0x41
#define	MPU6050REG_TEMP_OUT_L		0x42
#define	MPU6050REG_GYRO_XOUT_H		0x43
#define	MPU6050REG_GYRO_XOUT_L		0x44
#define	MPU6050REG_GYRO_YOUT_H		0x45
#define	MPU6050REG_GYRO_YOUT_L		0x46
#define	MPU6050REG_GYRO_ZOUT_H		0x47
#define	MPU6050REG_GYRO_ZOUT_L		0x48
#define	MPU6050REG_PWR_MGMT_1		0x6B
#define	MPU6050REG_WHO_AM_I			0x75

#define MPU6050_WHO_AM_I_VALUE		0x68

#define BITS_DLPF_CFG_256HZ_NOLPF2	0x00
#define BITS_DLPF_CFG_188HZ			0x01
#define BITS_DLPF_CFG_98HZ			0x02
#define BITS_DLPF_CFG_42HZ			0x03
#define BITS_DLPF_CFG_20HZ			0x04
#define BITS_DLPF_CFG_10HZ			0x05
#define BITS_DLPF_CFG_5HZ			0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF	0x07
#define BITS_DLPF_CFG_MASK			0x07

static struct i2c_client *mpu6050_i2c_client;
dev_t devno;
struct cdev *cdev;
struct device *device;
struct class *cls;

struct mpu6050_report_s {
	int16_t acc[3];
	int16_t temp;
	int16_t gyro[3];
};

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END
};

static bool mpu6050_i2c_test(struct i2c_client *client)
{
	int retry;

	retry = i2c_smbus_read_byte_data(client, MPU6050REG_WHO_AM_I);
	printk(KERN_INFO "[mpu6050]ID=0x%x\n", retry);

	if (retry == 0x68)
		return true;
	else
		return false;
}

static int mpu6050_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

static int mpu6050_init_client(struct i2c_client *client)
{
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);
	if (i2c_smbus_read_byte_data(client, MPU6050REG_WHO_AM_I) != 0x68)
		return -1;

	result = i2c_smbus_write_byte_data(client, MPU6050REG_PWR_MGMT_1, 0x80);
	if (result < 0) {
		printk(KERN_INFO "==%s== set MPU6050REG_PWR_MGMT_1 failed\r\n",
		       __func__);
		return -1;
	}
	mdelay(50);
	result = i2c_smbus_write_byte_data(client, MPU6050REG_SMPLRT_DIV, 0x00);
	if (result < 0) {
		printk(KERN_INFO "==%s== set MPU6050REG_SMPLRT_DIV failed\r\n",
		       __func__);
		return -1;
	}
	result = i2c_smbus_write_byte_data(client, MPU6050REG_PWR_MGMT_1, 0x03);
	if (result < 0) {
		printk(KERN_INFO "==%s== set MPU6050REG_PWR_MGMT_1 failed\r\n",
		       __func__);
		return -1;
	}
	result =
	    i2c_smbus_write_byte_data(client, MPU6050REG_CONFIG,
				      BITS_DLPF_CFG_20HZ);
	if (result < 0) {
		printk(KERN_INFO "==%s== set MPU6050REG_CONFIG failed\r\n",
		       __func__);
		return -1;
	}
	result =
	    i2c_smbus_write_byte_data(client, MPU6050REG_GYRO_CONFIG, 0x18);
	if (result < 0) {
		printk(KERN_INFO "==%s== set MPU6050REG_GYRO_CONFIG failed\r\n",
		       __func__);
		return -1;
	}
	result =
	    i2c_smbus_write_byte_data(client, MPU6050REG_ACCEL_CONFIG, 0x10);
	if (result < 0) {
		printk(KERN_INFO
		       "==%s== set MPU6050REG_ACCEL_CONFIG failed\r\n",
		       __func__);
		return -1;
	}

	return 0;
}

static int mpu6050_measure(struct mpu6050_report_s *pf)
{
	int result;
	uint8_t buff[14];

	result =
	    i2c_smbus_read_i2c_block_data(mpu6050_i2c_client,
					  MPU6050REG_ACCEL_XOUT_H, 14, buff);
	if (result < 0) {
		printk(KERN_INFO
		       "==%s== get MPU6050REG_ACCEL_XOUT_H failed\r\n",
		       __func__);
		return -1;
	}

	pf->acc[0] = buff[0] << 8 | buff[1];
	pf->acc[1] = buff[2] << 8 | buff[3];
	pf->acc[2] = buff[4] << 8 | buff[5];
	pf->temp = buff[6] << 8 | buff[7];
	pf->gyro[0] = buff[8] << 8 | buff[9];
	pf->gyro[1] = buff[10] << 8 | buff[11];
	pf->gyro[2] = buff[12] << 8 | buff[13];

	return 0;
}

int mpu6050_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (mpu6050_i2c_test(mpu6050_i2c_client) == false)
		return -1;
	mpu6050_init_client(mpu6050_i2c_client);
	return 0;
}

int mpu6050_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t mpu6050_read(struct file *filp, char __user *buf, size_t size,
			    loff_t *ppos)
{
	struct mpu6050_report_s data;
	int result;

	result = mpu6050_measure(&data);
	if (result < 0)
		return -1;

	copy_to_user(buf, (void *)&data, sizeof(data));

	return 1;
}

static const struct file_operations mpu6050_fops = {
	.owner = THIS_MODULE,
	.read = mpu6050_read,
	.open = mpu6050_open,
	.release = mpu6050_release,
};

static int mpu6050_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (mpu6050_i2c_client == NULL)
		mpu6050_i2c_client = client;

	if (mpu6050_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int mpu6050_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id mpu6050_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static struct i2c_driver mpu6050_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = mpu6050_probe,
	.remove = mpu6050_remove,
	.id_table = mpu6050_id,
	.detect = mpu6050_detect,
	.address_list = normal_i2c,
};

static int __init mpu6050_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&mpu6050_driver);
	if (ret < 0) {
		printk(KERN_INFO "add mpu6050 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "mpu6050");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &mpu6050_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "mpu6050");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n", __func__,
		       ret);

	}

	device = device_create(cls, NULL, devno, NULL, "mpu6050");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n", __func__,
		       ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit mpu6050_exit(void)
{
	printk(KERN_INFO "remove mpu6050 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&mpu6050_driver);
}

module_init(mpu6050_init);
module_exit(mpu6050_exit);

MODULE_AUTHOR("Lv Yuanzhou <lvyuanzhou@allwinnertech.com.com>");
MODULE_DESCRIPTION("mpu6050 3-Axis Orientation/Motion Detection Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
