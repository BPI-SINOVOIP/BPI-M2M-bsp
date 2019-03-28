/*
 *  a30e.c - Linux kernel modules for 3-Axis Orientation/Motion
 *  Detection Sensor
 *
 *  Copyright (C) 2009-2010 Freescale Semiconductor Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/input-polldev.h>
#include <linux/device.h>
#include <linux/init-input.h>

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_PM)
#include <linux/pm.h>
#endif

/*
 * Defines
 */
#define assert(expr)\
	do { if (!(expr)) {\
		printk(KERN_ERR "Assertion failed! %s,%d,%s,%s\n",\
			__FILE__, __LINE__, __func__, #expr);\
	}; } while (0)

/*¸ß ÖÐ µÍ ¹Ø±Õvalue 10000 means turn off collision check*/
static short threhold[4] = {5, 8, 11, 20};

static unsigned int	g_threshold;

#define A30E_DRV_NAME         "a3oe"
#define SENSOR_NAME           A30E_DRV_NAME
#define A30E_XOUT             0x00
#define A30E_YOUT             0x01
#define A30E_ZOUT             0x02
#define A30E_TILT             0x03
#define A30E_SRST             0x04
#define A30E_SPCNT            0x05
#define A30E_INTSU            0x06
#define A30E_MODE             0x07
#define A30E_SR               0x08

#define A30E_IIC_SLAVE_ADDR		0x1d

#define POLL_INTERVAL_MAX        500
#define POLL_INTERVAL            200
#define INPUT_FUZZ               4
#define INPUT_FLAT               2

#define MK_A30E_SR(FILT, AWSR, AMSR)\
	(FILT<<5 | AWSR<<3 | AMSR)

#define MK_A30E_MODE(IAH, IPP, SCPS, ASE, AWE, TON, MODE)\
	(IAH<<7 | IPP<<6 | SCPS<<5 | ASE<<4 | AWE<<3 | TON<<2 | MODE)

#define MK_A30E_INTSU(SHINTX, SHINTY, SHINTZ, GINT, ASINT, PDINT, PLINT, FBINT)\
	(SHINTX<<7 | SHINTY<<6 | SHINTZ<<5 | GINT<<4 | ASINT<<3 | PDINT<<2 | PLINT<<1 | FBINT)

#define MODE_CHANGE_DELAY_MS 100

static struct i2c_client *a30e_i2c_client;

static struct a30e_data_s {
	struct i2c_client       *client;
	struct input_polled_dev *pollDev;
	struct mutex interval_mutex;
	struct mutex init_mutex;


#if defined(CONFIG_PM) || defined(CONFIG_HAS_EARLYSUSPEND)
	volatile int suspend_indator;
#endif
} a30e_data;

static  struct input_polled_dev *a30e_idev;

/* Addresses to scan */
static const unsigned short normal_i2c[2] = {A30E_IIC_SLAVE_ADDR, I2C_CLIENT_END};
static __u32 twi_id ;
static struct sensor_config_info gsensor_info = {
	.input_type = GSENSOR_TYPE,
};

enum {
	DEBUG_INIT = 1U << 0,
	DEBUG_CONTROL_INFO = 1U << 1,
	DEBUG_DATA_INFO = 1U << 2,
	DEBUG_SUSPEND = 1U << 3,
};
/*DEBUG_SUSPEND | DEBUG_DATA_INFO | DEBUG_CONTROL_INFO | DEBUG_INIT;*/
static u32 debug_mask ;
#define dprintk(level_mask, fmt, arg...) \
	do { if (unlikely(debug_mask & level_mask)) \
			printk(KERN_DEBUG fmt , ## arg);	} while (0)

module_param_named(debug_mask, debug_mask, int, 0644);

#ifdef CONFIG_PM
static void a30e_init_events(struct work_struct *work);
static struct workqueue_struct *a30e_resume_wq;
static struct workqueue_struct *a30e_init_wq;

static DECLARE_WORK(a30e_resume_work, a30e_resume_events);
#endif

/*Function as i2c_master_send, and return 1 if operation is successful.*/
static int i2c_write_bytes(struct i2c_client *client, uint8_t *data, uint16_t len)
{
	struct i2c_msg msg;
	int ret = -1;

	msg.flags = !I2C_M_RD;
	msg.addr = client->addr;
	msg.len = len;
	msg.buf = data;

	ret = i2c_transfer(client->adapter, &msg, 1);
	return ret;
}
static bool gsensor_i2c_test(struct i2c_client *client)
{
	int ret, retry;
	uint8_t test_data[1] = { 0 };

	for (retry = 0 ; retry < 2; retry++) {
		ret = i2c_write_bytes(client, test_data, 1);
		if (ret == 1)
			break;
		msleep(5);
	}

	return ret == 1 ? true : false;
}

/**
 * gsensor_detect - Device detection callback for automatic device creation
 * return value:
 *                    = 0; success;
 *                    < 0; err
 */
static int gsensor_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int ret;

	dprintk(DEBUG_INIT, "%s enter \n", __func__);

	ret = i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA);
	if (!ret)
	return -ENODEV;

	if (twi_id == adapter->nr) {
		pr_info("%s: addr= %x\n", __func__, client->addr);
		ret = gsensor_i2c_test(client);
		if (!ret) {
			pr_info("%s:I2C connection might be something wrong or maybe the other gsensor equipment! \n", __func__);
			return -ENODEV;
		} else {
			pr_info("I2C connection sucess!\n");
			strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
			return 0;
		}
	} else {
		return -ENODEV;
	}
}

static int a30e_read_xyz(int idx, s8 *pf)
{
	s32 result;

	assert(a30e_i2c_client);
	result = i2c_smbus_read_byte_data(a30e_i2c_client, idx+A30E_XOUT);
	assert(result >= 0);
	if (result & (1 << 6)) {
		dprintk(DEBUG_DATA_INFO, "a30e read new data\n");
		return -1;
	}

	*pf = (result&(1<<5)) ? (result|(~0x0000003f)) : (result&0x0000003f);

	return 0;
}

static ssize_t a30e_delay_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long data;
	int error;
	struct i2c_client *client = a30e_i2c_client;
	struct a30e_data_s *a30e = NULL;

	a30e = i2c_get_clientdata(client);

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (data > POLL_INTERVAL_MAX)
		data = POLL_INTERVAL_MAX;

	mutex_lock(&a30e->interval_mutex);
	a30e->pollDev->poll_interval = data;
	mutex_unlock(&a30e->interval_mutex);

	return count;
}

static ssize_t a30e_value_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int i;
	int result = 0;
	s8 xyz[3];
	s16 x, y, z;

	for (i = 0; i < 3; i++) {
		result = a30e_read_xyz(i, &xyz[i]);
		if (result < 0) {
			dprintk(DEBUG_DATA_INFO, "a30e read data failed\n");
			return 0;
		}
	}

	/* convert signed 8bits to signed 16bits */
	x = (((short)xyz[0]) << 8) >> 8;
	y = (((short)xyz[1]) << 8) >> 8;
	z = (((short)xyz[2]) << 8) >> 8;

	return sprintf(buf, "x= %d y= %d z= %d\n", x, y, z);

}

static ssize_t a30e_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long data;
	int error;

	error = strict_strtoul(buf, 10, &data);

	if (error) {
		pr_err("%s strict_strtoul error\n", __FUNCTION__);
		goto exit;
	}

	if (data) {
			a30e_idev->input->open(a30e_idev->input);

		dprintk(DEBUG_CONTROL_INFO, "a30e enable setting active \n");
	} else {
		a30e_idev->input->close(a30e_idev->input);
		dprintk(DEBUG_CONTROL_INFO, "a30e enable setting inactive \n");
	}

	return count;

exit:
	return error;
}

static ssize_t a30e_threshold_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", g_threshold);

}

static ssize_t a30e_threshold_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	long data;
	int error;

	error = strict_strtol(buf, 10, &data);
	if (error) {
		pr_err("%s strict_strtoul error\n", __FUNCTION__);
		goto exit;
	}

	if (data >= 0 && data <= 3) {
		g_threshold = threhold[data];
	} else {
		dprintk(DEBUG_CONTROL_INFO, "data must be 0~3\n");
	}

	return count;

exit:
	return error;
}

static DEVICE_ATTR(enable, S_IRUGO|S_IWUSR|S_IWGRP,
		NULL, a30e_enable_store);

static DEVICE_ATTR(value, S_IRUGO|S_IWUSR|S_IWGRP,
		a30e_value_show, NULL);

static DEVICE_ATTR(delay, S_IRUGO|S_IWUSR|S_IWGRP,
		NULL, a30e_delay_store);

static DEVICE_ATTR(threshold, S_IRUGO|S_IWUSR|S_IWGRP,
		a30e_threshold_show, a30e_threshold_store);


static struct attribute *a30e_attributes[] = {
	&dev_attr_value.attr,
	&dev_attr_enable.attr,
	&dev_attr_delay.attr,
	&dev_attr_threshold.attr,
	NULL
};

static struct attribute_group a30e_attribute_group = {
	.attrs = a30e_attributes
};


/*
 * Initialization function
 */

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_PM)

static int a30e_init_client(struct i2c_client *client)
{
	int result;

	a30e_i2c_client = client;

	mutex_lock(&a30e_data.init_mutex);
	if (0) {
		/*
		 * Probe the device. We try to set the device to Test Mode and then to
		 * write & verify XYZ registers
		 */
		result = i2c_smbus_write_byte_data(client, A30E_MODE, MK_A30E_MODE(0, 0, 0, 0, 0, 1, 0));
		assert(result == 0);
		mdelay(MODE_CHANGE_DELAY_MS);

		result = i2c_smbus_write_byte_data(client, A30E_XOUT, 0x2a);
		assert(result == 0);

		result = i2c_smbus_write_byte_data(client, A30E_YOUT, 0x15);
		assert(result == 0);

		result = i2c_smbus_write_byte_data(client, A30E_ZOUT, 0x3f);
		assert(result == 0);

		result = i2c_smbus_read_byte_data(client, A30E_XOUT);

		result = i2c_smbus_read_byte_data(client, A30E_YOUT);

		result = i2c_smbus_read_byte_data(client, A30E_ZOUT);
		assert(result = 0x3f);
	}
	/* Enable Orientation Detection Logic	, enter standby*/
	result = i2c_smbus_write_byte_data(client,
		A30E_MODE, MK_A30E_MODE(0, 0, 0, 0, 0, 0, 0));
	assert(result == 0);

	result = i2c_smbus_write_byte_data(client,
		A30E_SR, MK_A30E_SR(2, 2, 1));
	assert(result == 0);

	result = i2c_smbus_write_byte_data(client,
		A30E_INTSU, MK_A30E_INTSU(0, 0, 0, 0, 1, 0, 1, 1));
	assert(result == 0);

	result = i2c_smbus_write_byte_data(client,
		A30E_SPCNT, 0xA0);
	assert(result == 0);

	mutex_unlock(&a30e_data.init_mutex);

	return result;
}
#endif

static int i2c_read_block_data(unsigned char base_addr, unsigned char count, unsigned char *data)
{
	int i = 0;

	for (i = 0; i < count; i++) {
		*(data+i) = i2c_smbus_read_byte_data(a30e_i2c_client, base_addr+i);
	}

	return count;
}

int a30e_register_read_continuously(unsigned char addr, unsigned char count, unsigned char *data)
{
    int     res = 0;

    res = (count == i2c_read_block_data(addr, count, data)) ? 0 : 1;

    return res;
}

int a30e_register_read(unsigned char  addr, u8 *data)
{
	int   res = 0;

	*data = i2c_smbus_read_byte_data(a30e_i2c_client, addr);

	return res;
}

int a30e_register_write(u8 addr, u8 data)
{
    int res = 0;

    res = i2c_smbus_write_byte_data(a30e_i2c_client, addr, data);

    return res;
}

int a30e_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data)
{
    int     res = 0;
    unsigned char      tmp_data;

    res = a30e_register_read(addr, &tmp_data);
    if (res) {
		return res;
    }

    tmp_data &= ~mask;
    tmp_data |= data & mask;
    res = a30e_register_write(addr, tmp_data);

	return res;
}

static int a30e_read_data(short *x, short *y, short *z)
{
	int i = 0;
  unsigned char	tmp_data[6] = {0};
	unsigned char	reg27 = 0;

	while (!reg27) {
		a30e_register_read(0x27, &reg27);
		reg27 &= 0x08;
	}

    if (a30e_register_read_continuously(0x28, 6, tmp_data) != 0) {
				return -1;
    }

	for (i = 0; i < 6; i++) {
		if (tmp_data[i]&0x80) {
			tmp_data[i] = (~tmp_data[i])+1;
		}
	}

	*x = tmp_data[1];
	*y = tmp_data[3];
	*z = tmp_data[5];
	return 0;
}

static int gs_readdata(void)
{
	static short prev_x, prev_y, prev_z;
	short x = 0, y = 0, z = 0;
	static char b_is_first ;

	if (a30e_read_data(&x, &y, &z))
		return -1;
	if (b_is_first == 0) {
		prev_x = x;
		prev_y = y;
		prev_z = z;
		b_is_first = 1;
		return 0;
	}

	if ((abs(x - prev_x) > g_threshold) || (abs(y - prev_y) > g_threshold) || (abs(z - prev_z) > g_threshold)) {
			printk("hit ******** \n");
			/* report event */
			input_report_abs(a30e_idev->input, ABS_Y, y);
			input_report_abs(a30e_idev->input, ABS_Y, y);
			input_report_abs(a30e_idev->input, ABS_Z, z);
			input_sync(a30e_idev->input);
			b_is_first = 0;
			printk("hit input_sync \n");
	}
	prev_x = x;
	prev_y = y;
	prev_z = z;
	return 0;
}

static void a30e_dev_poll(struct input_polled_dev *dev)
{
	gs_readdata();
}

static void polled_dev_open(struct input_polled_dev *dev)
{
	unsigned char data = 0;
	printk("<%s>!\n", __func__);
	a30e_register_write(0x24, 0x88);
	a30e_register_write(0x20, 0x47);
	a30e_register_read(0x26, &data);
	a30e_register_write(0x21, 0x09);
	a30e_register_write(0x25, 0x00);
	a30e_register_write(0x30, 0x2a);
	a30e_register_write(0x32, 0x40);
	a30e_register_write(0x33, 0x03);
	a30e_register_write(0x22, 0x40);
	a30e_register_write(0x24, 0x88);
}

static void polled_dev_close(struct input_polled_dev *dev)
{
	printk("<%s>!\n", __func__);
	a30e_register_write(0x24, 0x80);
	a30e_register_write(0x22, 0x00);
	a30e_register_write(0x30, 0x00);
}

static int a30e_reg_init(void)
{
	int          res = 0;
	unsigned char data = 0;

	printk("------mir3da init start\r\n");
	a30e_register_write(0x22, 0x00);
	a30e_register_write(0x24, 0x80);
	a30e_register_read(0x0f, &data);
	printk("------mir3da chip id = %x-----\r\n", data);
	a30e_register_write(0x20, 0x47);
	a30e_register_read(0x26, &data);
	a30e_register_write(0x21, 0x09);
	a30e_register_write(0x23, 0x90);
	a30e_register_write(0x25, 0x00);
	a30e_register_write(0x30, 0x00);
	return res;
}

static int a30e_probe(struct i2c_client *client,
				   const struct i2c_device_id *id)
{
	int result, res;
	struct input_dev *idev;
	struct i2c_adapter *adapter;
	struct a30e_data_s *data = &a30e_data;
  printk("%s ======enter\n", __FUNCTION__);
	dprintk(DEBUG_INIT, "a30e probe\n");
	a30e_i2c_client = client;
	adapter = to_i2c_adapter(client->dev.parent);
	result = i2c_check_functionality(adapter,
					 I2C_FUNC_SMBUS_BYTE |
					 I2C_FUNC_SMBUS_BYTE_DATA);
	assert(result);

	dprintk(DEBUG_INIT, "<%s> a30e_init_client result %d\n", __func__, result);

	dev_info(&client->dev, "build time %s %s\n", __DATE__, __TIME__);

	g_threshold = threhold[0];

	res = a30e_reg_init();
	printk("mir3da_init ret  = %d \n", res);

	/*input poll device register */
	a30e_idev = input_allocate_polled_device();
	if (!a30e_idev) {
		dev_err(&client->dev, "alloc poll device failed!\n");
		result = -ENOMEM;
		return result;
	}
	a30e_idev->poll = a30e_dev_poll;
	a30e_idev->open = polled_dev_open;
	a30e_idev->close = polled_dev_close;
	a30e_idev->poll_interval = POLL_INTERVAL;
	a30e_idev->poll_interval_max = POLL_INTERVAL_MAX;
	idev = a30e_idev->input;
	idev->name = A30E_DRV_NAME;
	idev->id.bustype = BUS_I2C;
	set_bit(EV_ABS, idev->evbit);
	mutex_init(&data->interval_mutex);
	mutex_init(&data->init_mutex);

	/*set_bit(ABS_X, idev->absbit);
		set_bit(ABS_Y, idev->absbit);
		set_bit(ABS_Z, idev->absbit);*/

	input_set_abs_params(idev, ABS_X, -16384, 16383, INPUT_FUZZ, INPUT_FLAT);
  input_set_abs_params(idev, ABS_Y, -16384, 16383, INPUT_FUZZ, INPUT_FLAT);
  input_set_abs_params(idev, ABS_Z, -16384, 16383, INPUT_FUZZ, INPUT_FLAT);

	result = input_register_polled_device(a30e_idev);
	if (result) {
		dev_err(&client->dev, "register poll device failed!\n");
		return result;
	}

	result = sysfs_create_group(&a30e_idev->input->dev.kobj, &a30e_attribute_group);
	if (result) {
		dev_err(&client->dev, "create sys failed\n");
	}

	data->client  = client;
	data->pollDev = a30e_idev;
	i2c_set_clientdata(client, data);


#ifdef CONFIG_PM
	/* Initialize the work queue */
	queue_work(a30e_init_wq, &a30e_init_work);
	a30e_resume_wq = create_singlethread_workqueue("a30e_resume");
	if (a30e_resume_wq == NULL) {
		printk("create a30e_resume_wq fail!\n");
		return -ENOMEM;
	}
#endif

	dprintk(DEBUG_INIT, "a30e probe end\n");
	printk("a30e probe end\n");

	return result;
}

static int a30e_remove(struct i2c_client *client)
{
	int result;

	mutex_lock(&a30e_data.init_mutex);
	result = i2c_smbus_write_byte_data(client, A30E_MODE, MK_A30E_MODE(0, 0, 0, 0, 0, 0, 0));
	assert(result == 0);
	mutex_unlock(&a30e_data.init_mutex);
#ifdef CONFIG_PM
	cancel_work_sync(&a30e_resume_work);
	destroy_workqueue(a30e_resume_wq);
#endif
	sysfs_remove_group(&a30e_idev->input->dev.kobj, &a30e_attribute_group);
	a30e_idev->input->close(a30e_idev->input);
	input_unregister_polled_device(a30e_idev);
	input_free_polled_device(a30e_idev);

#ifdef CONFIG_PM
	cancel_work_sync(&a30e_init_work);
	destroy_workqueue(a30e_init_wq);
#endif
	i2c_set_clientdata(client, NULL);

	return result;
}

#ifdef CONFIG_PM
static void a30e_resume_events (struct work_struct *work)
{
	int result = 0;
	dprintk(DEBUG_INIT, "a30e device init\n");
	result = a30e_init_client(a30e_i2c_client);
	assert(result == 0);

	mutex_lock(&a30e_data.init_mutex);
	a30e_data.suspend_indator = 0;
	result = i2c_smbus_write_byte_data(a30e_i2c_client,
		A30E_MODE, MK_A30E_MODE(0, 1, 0, 0, 0, 0, 1));
	mutex_unlock(&a30e_data.init_mutex);
	assert(result == 0);

	a30e_idev->input->open(a30e_idev->input);

	dprintk(DEBUG_INIT, "a30e device init end\n");
	return;
}

static int a30e_resume(struct i2c_client *client)
{
	int result = 0;
	dprintk(DEBUG_SUSPEND, "a30e resume\n");

	if (SUPER_STANDBY == standby_type) {
		queue_work(a30e_resume_wq, &a30e_resume_work);
	}
	if (SUPER_STANDBY == standby_type) {
		mutex_lock(&a30e_data.init_mutex);
		a30e_data.suspend_indator = 0;
		result = i2c_smbus_write_byte_data(a30e_i2c_client,
			A30E_MODE, MK_A30E_MODE(0, 1, 0, 0, 0, 0, 1));
		mutex_unlock(&a30e_data.init_mutex);
		assert(result == 0);
		a30e_idev->input->open(a30e_idev->input);
	}
	return result;
}

static int a30e_suspend(struct i2c_client *client, pm_message_t mesg)
{
	int result;
	dprintk(DEBUG_SUSPEND, "a30e suspend\n");

	flush_workqueue(a30e_resume_wq);

	a30e_idev->input->close(a30e_idev->input);

	mutex_lock(&a30e_data.init_mutex);
	a30e_data.suspend_indator = 1;
	result = i2c_smbus_write_byte_data(a30e_i2c_client,
		A30E_MODE, MK_A30E_MODE(0, 0, 0, 0, 0, 0, 0));
	mutex_unlock(&a30e_data.init_mutex);
	assert(result == 0);
	return result;
}
#endif


static const struct i2c_device_id a30e_id[] = {
	{ A30E_DRV_NAME, 1 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, a30e_id);

static struct i2c_driver a30e_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name	= A30E_DRV_NAME,
		.owner	= THIS_MODULE,
	},
	.probe	= a30e_probe,
	.remove	= a30e_remove,
#ifdef CONFIG_PM
	.suspend = a30e_suspend,
	.resume = a30e_resume,
#endif
	.id_table = a30e_id,
	.detect = gsensor_detect,
	.address_list = normal_i2c,
};

static int __init a30e_init(void)
{
	int ret = -1;
	printk("======%s=========. \n", __func__);
	gsensor_info.input_type = GSENSOR_TYPE;
	dprintk(DEBUG_INIT, "%s i2c twi is %d \n", __func__, twi_id);
	ret = i2c_add_driver(&a30e_driver);
	if (ret < 0) {
		printk(KERN_INFO "add a30e i2c driver failed\n");
		return -ENODEV;
	}
	dprintk(DEBUG_INIT, "add a30e i2c driver\n");
	return ret;
}

static void __exit a30e_exit(void)
{
	printk(KERN_INFO "remove a30e i2c driver.\n");
	i2c_del_driver(&a30e_driver);
}

MODULE_AUTHOR("Wang Gang");
MODULE_DESCRIPTION("A30E 3-Axis Orientation/Motion Detection Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");

module_init(a30e_init);
module_exit(a30e_exit);
