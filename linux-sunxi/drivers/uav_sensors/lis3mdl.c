/*
 * drivers/uav_sensors/lis3mdl.c
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

#define assert(expr) ( \
	if (!(expr)) {\
		printk(KERN_ERR "Assertion failed! %s,%d,%s,%s\n",\
				__FILE__, __LINE__, __func__, #expr);\
	} \
)

#define SENSOR_NAME "lis3mdl"
#define SENSOR_I2C_SLAVE_ADDRESS	0x1C

/******************************************************************************/
/***************** START MAGNETIC SENSOR REGISTER MAPPING  ********************/
/******************************************************************************/

/**
  * @brief Device identifier register.
  * \code
  * Read
  * Default value:
  * 7:0 This read-only register contains the device identifier
  * \endcode
*/
#define LIS3MDL_M_WHO_AM_I_ADDR                             0x0F
#define LIS3MDL_M_WHO_AM_I_VALUE                            0x3D

/**
 * @brief Magnetic sensor Control Register 1
 * \code
 * Read/write
 * Default value: 0x10
 * [7] TEMP_COMP: Temperature compensation enable
 * [6:5] OM1-0: X and Y axes operative mode selection
 * [4:2] DO2-0: Output data rate selection
 * [1] This bit must be set to ‘0’ for the correct operation of the device
 * [0] ST: Self-test enable
 * \endcode
 */
#define LIS3MDL_M_CTRL_REG1_M                               0x20

/**
 * @brief Magnetic sensor Control Register 2
 * \code
 * Read/write
 * Default value: 0x00
 * [7] These bits must be set to ‘0’ for the correct operation of the device
 * [6:5] FS1-0: Full-scale configuration
 * [4] These bits must be set to ‘0’ for the correct operation of the device
 * [3] REBOOT: Reboot memory content
 * [2] SOFT_RST: Configuration registers and user register reset function
 * [1:0] These bits must be set to ‘0’ for the correct operation of the device
 * \endcode
 */
#define LIS3MDL_M_CTRL_REG2_M                               0x21

/**
 * @brief Magnetic sensor Control Register 3
 * \code
 * Read/write
 * Default value: 0x03
 * [7] I2C_DISABLE: Disable I2C interface
 * [6] These bits must be set to ‘0’ for the correct operation of the device
 * [5] LP: Low-power mode configuration
 * [4:3] These bits must be set to ‘0’ for the correct operation of the device
 * [2] SIM: SPI Serial Interface mode selection
 * [1:0] MD1-0: Operating mode selection
 * \endcode
 */
#define LIS3MDL_M_CTRL_REG3_M                               0x22

/**
 * @brief Magnetic sensor data (LSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_X_L_M                                 0x28

/**
 * @brief Magnetic sensor data (MSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_X_H_M                                  0x29

/**
 * @brief Magnetic sensor data (LSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_Y_L_M                                  0x2A

/**
 * @brief Magnetic sensor data (MSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_Y_H_M                                  0x2B

/**
 * @brief Magnetic sensor data (LSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_Z_L_M                                  0x2C

/**
 * @brief Magnetic sensor data (MSB)
 * \code
 * Read
 * \endcode
 */
#define LIS3MDL_M_OUT_Z_H_M                                  0x2D

/**
 * @brief Magnetic sensor Interrupt config register
 * \code
 * Read/write
 * Default value: 0x00
 * [7] XIEN: Enable interrupt generation on X axis
 * [6] YIEN: Enable interrupt generation on Y axis
 * [5] ZIEN: Enable interrupt generation on Z axis
 * [4:3] Must be 0
 * [2] IEA: Interrupt active configuration on INT
 * [1] LIR: Latch interrupt request
 * [0] IEN: Interrupt enable on INT pin
 * \endcode
 */
#define LIS3MDL_M_INT_CFG                                   0x30

/**
 * @brief Magnetic sensor Interrupt source register
 * \code
 * Read/write
 * Default value: 0x00
 * [7] PTH_X: Value on X-axis exceeds the threshold on the positive side
 * [6] PTH_Y: Value on Y-axis exceeds the threshold on the positive side
 * [5] PTH_Z: Value on Z-axis exceeds the threshold on the positive side
 * [4] NTH_X: Value on X-axis exceeds the threshold on the negative side
 * [3] NTH_Y: Value on Y-axis exceeds the threshold on the negative side
 * [2] NTH_Z: Value on Z-axis exceeds the threshold on the negative side
 * [1] MROI: Internal measurement range overflow on magnetic value
 * [0] INT: This bit signals when interrupt event occours
 * \endcode
 */
#define LIS3MDL_M_INT_SRC                                   0x31

/**
 * @brief Magnetic sensor Interrupt threshold register low
 * \code
 * Read/write
 * Default value: 0x00
 * [7:0] THS7-0: Least 8 significant bits of interrupt threshold
 * \endcode
 */
#define LIS3MDL_M_INT_THS_L_M                               0x32

/**
 * @brief Magnetic sensor Interrupt threshold register high
 * \code
 * Read/write
 * Default value: 0x00
 * [7] Must be 0
 * [6:0] THS14-8: Most 7 significant bits of interrupt threshold
 * \endcode
 */
#define LIS3MDL_M_INT_THS_H_M                               0x33

/******************************************************************************/
/******************* END MAGNETIC SENSOR REGISTER MAPPING  ********************/
/******************************************************************************/

/**
 * @brief Device Address
 */

#define LIS3MDL_M_MEMS_ADDRESS                              0x1C

/**
 * @brief Device Identifier. Default value of the WHO_AM_I register.
 */
#define I_AM_LIS3MDL_M                                  ((uint8_t)0x3D)


/** @defgroup LIS3MDL_M Temperature compensation enable selection CTRL_REG1_M
 * @{
 */
#define LIS3MDL_M_TEMP_COMP_DISABLE                     ((uint8_t)0x00)
#define LIS3MDL_M_TEMP_COMP_ENABLE                      ((uint8_t)0x80)

#define LIS3MDL_M_TEMP_COMP_MASK                        ((uint8_t)0x80)

/** @defgroup LIS3MDL_M X and Y axes operative mode selection CTRL_REG1_M
 * @{
 */
#define LIS3MDL_M_OM_LP                                 ((uint8_t)0x00)
#define LIS3MDL_M_OM_MP                                 ((uint8_t)0x20)
#define LIS3MDL_M_OM_HP                                 ((uint8_t)0x40)
#define LIS3MDL_M_OM_UHP                                ((uint8_t)0x60)

#define LIS3MDL_M_OM_MASK                               ((uint8_t)0x60)

/** @defgroup LIS3MDL_M Output data rate selection CTRL_REG1_M
 * @{
 */
#define LIS3MDL_M_DO_0_625                              ((uint8_t)0x00)
#define LIS3MDL_M_DO_1_25                               ((uint8_t)0x04)
#define LIS3MDL_M_DO_2_5                                ((uint8_t)0x08)
#define LIS3MDL_M_DO_5                                  ((uint8_t)0x0C)
#define LIS3MDL_M_DO_10                                 ((uint8_t)0x10)
#define LIS3MDL_M_DO_20                                 ((uint8_t)0x14)
#define LIS3MDL_M_DO_40                                 ((uint8_t)0x18)
#define LIS3MDL_M_DO_80                                 ((uint8_t)0x1C)

#define LIS3MDL_M_DO_MASK                               ((uint8_t)0x1C)

/** @defgroup LIS3MDL_M Self-test enable selection CTRL_REG1_M
 * @{
 */
#define LIS3MDL_M_ST_DISABLE                            ((uint8_t)0x00)
#define LIS3MDL_M_ST_ENABLE                             ((uint8_t)0x01)

#define LIS3MDL_M_ST_MASK                               ((uint8_t)0x01)

/** @defgroup LIS3MDL_M Full scale selection CTRL_REG2_M
 * @{
 */
#define LIS3MDL_M_FS_4                                  ((uint8_t)0x00)
#define LIS3MDL_M_FS_8                                  ((uint8_t)0x20)
#define LIS3MDL_M_FS_12                                 ((uint8_t)0x40)
#define LIS3MDL_M_FS_16                                 ((uint8_t)0x60)

#define LIS3MDL_M_FS_MASK                               ((uint8_t)0x60)

/** @defgroup LIS3MDL_M Reboot memory selection CTRL_REG2_M
 * @{
 */
#define LIS3MDL_M_REBOOT_NORMAL                         ((uint8_t)0x00)
#define LIS3MDL_M_REBOOT_MEM_CONTENT                    ((uint8_t)0x08)

#define LIS3MDL_M_REBOOT_MASK                           ((uint8_t)0x08)

/** @defgroup LIS3MDL_M Configuration registers and user register reset
CTRL_REG2_M
 * @{
 */
#define LIS3MDL_M_SOFT_RST_DEFAULT                      ((uint8_t)0x00)
#define LIS3MDL_M_SOFT_RST_RESET                        ((uint8_t)0x04)

#define LIS3MDL_M_SOFT_RST_MASK                         ((uint8_t)0x04)

/** @defgroup LIS3MDL_M Disable I2C interface selection CTRL_REG3_M
 * @{
 */
#define LIS3MDL_M_I2C_ENABLE                            ((uint8_t)0x00)
#define LIS3MDL_M_I2C_DISABLE                           ((uint8_t)0x80)

#define LIS3MDL_M_I2C_MASK                              ((uint8_t)0x80)

/** @defgroup LIS3MDL_M Low-power mode selection CTRL_REG3_M
 * @{
 */
#define LIS3MDL_M_LP_ENABLE                            ((uint8_t)0x00)
#define LIS3MDL_M_LP_DISABLE                           ((uint8_t)0x20)

#define LIS3MDL_M_LP_MASK                              ((uint8_t)0x20)

/** @defgroup LIS3MDL_M SPI Serial Interface mode selection CTRL_REG3_M
 * @{
 */
#define LIS3MDL_M_SPI_R_ENABLE                          ((uint8_t)0x00)
#define LIS3MDL_M_SPI_R_DISABLE                         ((uint8_t)0x40)

#define LIS3MDL_M_SPI_R_MASK                            ((uint8_t)0x40)

/** @defgroup LIS3MDL_M Operating mode selection CTRL_REG3_M
 * @{
 */
#define LIS3MDL_M_MD_CONTINUOUS                         ((uint8_t)0x00)
#define LIS3MDL_M_MD_SINGLE                             ((uint8_t)0x01)
#define LIS3MDL_M_MD_PD                                 ((uint8_t)0x02)

#define LIS3MDL_M_MD_MASK                               ((uint8_t)0x03)

/**
 * @}
 */

static struct i2c_client *lis3mdl_i2c_client;

struct lis3mdl_report_s {
	int16_t x;
	int16_t y;
	int16_t z;
};

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static bool lis3mdl_test(struct i2c_client *client)
{
	int retry;

	retry = i2c_smbus_read_byte_data(client, LIS3MDL_M_WHO_AM_I_ADDR);
	if (retry == LIS3MDL_M_WHO_AM_I_VALUE)
		return true;
	else
		return false;
}

static int lis3mdl_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

static int lis3mdl_init_client(struct i2c_client *client)
{
	int result;

	if (i2c_smbus_read_byte_data(client, LIS3MDL_M_WHO_AM_I_ADDR) !=
	    LIS3MDL_M_WHO_AM_I_VALUE) {
		return -1;
	}

	result = i2c_smbus_write_byte_data(client, LIS3MDL_M_CTRL_REG3_M, 0x00);

	result = i2c_smbus_write_byte_data(client, LIS3MDL_M_CTRL_REG1_M, 0x62);

	result = i2c_smbus_write_byte_data(client, LIS3MDL_M_CTRL_REG2_M, 0x20);

	return 0;
}

static int lis3mdl_measure(struct lis3mdl_report_s *pf)
{
	uint8_t buff[6];

	i2c_smbus_read_i2c_block_data(lis3mdl_i2c_client, LIS3MDL_M_OUT_X_L_M,
				      6, buff);
	pf->x = buff[1] << 8 | buff[0];
	pf->y = buff[3] << 8 | buff[2];
	pf->z = buff[5] << 8 | buff[4];

	return 0;
}

int lis3mdl_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "lis3mdl_test\n");
	if (lis3mdl_test(lis3mdl_i2c_client) == false) {
		printk(KERN_INFO "device lis3mdl not found\n");
		return -1;
	}
	printk(KERN_INFO "lis3mdl_init_client\n");
	lis3mdl_init_client(lis3mdl_i2c_client);
	printk(KERN_INFO "device lis3mdl open success\n");
	return 0;
}

int lis3mdl_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	return 0;
}

static ssize_t lis3mdl_read(struct file *filp, char __user *buf, size_t size,
			    loff_t *ppos)
{
	struct lis3mdl_report_s data;
	lis3mdl_measure(&data);
	copy_to_user(buf, (void *)&data, sizeof(data));
	return 1;
}

static const struct file_operations lis3mdl_fops = {
	.owner = THIS_MODULE,
	.read = lis3mdl_read,
	.open = lis3mdl_open,
	.release = lis3mdl_release,
};

static int lis3mdl_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	printk(KERN_INFO "lis3mdl_probe\n");
	if (lis3mdl_i2c_client == NULL)
		lis3mdl_i2c_client = client;

	if (lis3mdl_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int lis3mdl_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id lis3mdl_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, lis3mdl_id);

static struct i2c_driver lis3mdl_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,

		   },
	.probe = lis3mdl_probe,
	.remove = lis3mdl_remove,
	.id_table = lis3mdl_id,
	.detect = lis3mdl_detect,
	.address_list = normal_i2c,
};

static int __init lis3mdl_init(void)
{
	int ret = -1;
	int result;
	dev_t devno;
	struct cdev *cdev;
	struct device *device;
	struct class *cls;

	ret = i2c_add_driver(&lis3mdl_driver);
	if (ret < 0) {
		printk(KERN_INFO "add lis3mdl i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "lis3mdl");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &lis3mdl_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "lis3mdl");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n",
		__func__, ret);

	}

	device = device_create(cls, NULL, devno, NULL, "lis3mdl");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n",
		__func__, ret);
	}

	return ret;
}

static void __exit lis3mdl_exit(void)
{
	printk(KERN_INFO "remove lis3mdl i2c driver.\n");
	i2c_del_driver(&lis3mdl_driver);
}

module_init(lis3mdl_init);
module_exit(lis3mdl_exit);

MODULE_AUTHOR("Lv Yuanzhou <lvyuanzhou@allwinnertech.com.com>");
MODULE_DESCRIPTION("lis3mdl 3-Axis Orientation/Motion Detection Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
