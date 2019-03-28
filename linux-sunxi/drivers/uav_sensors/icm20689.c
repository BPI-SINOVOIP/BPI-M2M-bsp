#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/regulator/consumer.h>

#define SENSOR_NAME "icm20689"
#define SENSOR_I2C_SLAVE_ADDRESS	0x69
#define	ICM20689REG_SMPLRT_DIV		0x19
#define	ICM20689REG_CONFIG			0x1A
#define	ICM20689REG_GYRO_CONFIG		0x1B
#define	ICM20689REG_ACCEL_CONFIG		0x1C
#define	ICM20689REG_INT_PIN_CFG		0x37
#define	ICM20689REG_INT_ENABLE		0x38
#define ICM20689REG_INT_STATUS		0x3A
#define	ICM20689REG_ACCEL_XOUT_H		0x3B
#define	ICM20689REG_ACCEL_XOUT_L		0x3C
#define	ICM20689REG_ACCEL_YOUT_H		0x3D
#define	ICM20689REG_ACCEL_YOUT_L		0x3E
#define	ICM20689REG_ACCEL_ZOUT_H		0x3F
#define	ICM20689REG_ACCEL_ZOUT_L		0x40
#define	ICM20689REG_TEMP_OUT_H		0x41
#define	ICM20689REG_TEMP_OUT_L		0x42
#define	ICM20689REG_GYRO_XOUT_H		0x43
#define	ICM20689REG_GYRO_XOUT_L		0x44
#define	ICM20689REG_GYRO_YOUT_H		0x45
#define	ICM20689REG_GYRO_YOUT_L		0x46
#define	ICM20689REG_GYRO_ZOUT_H		0x47
#define	ICM20689REG_GYRO_ZOUT_L		0x48
#define	ICM20689REG_PWR_MGMT_1		0x6B
#define	ICM20689REG_WHO_AM_I		0x75

#define ICM20689_WHO_AM_I_VALUE		0x98

#define BITS_DLPF_CFG_256HZ_NOLPF2	0x00
#define BITS_DLPF_CFG_188HZ			0x01
#define BITS_DLPF_CFG_98HZ			0x02
#define BITS_DLPF_CFG_42HZ			0x03
#define BITS_DLPF_CFG_20HZ			0x04
#define BITS_DLPF_CFG_10HZ			0x05
#define BITS_DLPF_CFG_5HZ			0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF	0x07
#define BITS_DLPF_CFG_MASK			0x07

static struct i2c_client *icm20689_i2c_client;
dev_t devno;
struct cdev *cdev;
struct device *device;
struct class *cls;
struct regulator *vcc;

struct icm20689_report_s {
	int16_t acc[3];
	int16_t temp;
	int16_t gyro[3];
	long long ts;
};

static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END
};

static bool icm20689_i2c_test(struct i2c_client *client)
{
	int retry;

	retry = i2c_smbus_read_byte_data(client, ICM20689REG_WHO_AM_I);
	printk(KERN_INFO "[icm20689]ID=0x%x\n", retry);

	if (retry == 0x98)
		return true;
	else
		return false;
}

static int icm20689_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{

	struct i2c_adapter *adapter = client->adapter;

	if (2 != adapter->nr)
		return -ENODEV;

	pr_info("%s: addr=0x%x\n", __func__, client->addr);
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

static int icm20689_init_client(struct i2c_client *client)
{
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	result =
		i2c_smbus_write_byte_data(client, ICM20689REG_PWR_MGMT_1, 0x80);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_PWR_MGMT_1 failed\r\n", __func__);
		return -1;
	}
	mdelay(50);

	result =
		i2c_smbus_write_byte_data(client, ICM20689REG_PWR_MGMT_1, 0x03);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_PWR_MGMT_1 failed\r\n", __func__);
		return -1;
	}
	result =
		i2c_smbus_write_byte_data(client, ICM20689REG_SMPLRT_DIV, 0x04);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_SMPLRT_DIV failed\r\n", __func__);
		return -1;
	}
	result =
		i2c_smbus_write_byte_data
		(client, ICM20689REG_CONFIG, BITS_DLPF_CFG_5HZ);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_CONFIG failed\r\n", __func__);
		return -1;
	}
	result =
		i2c_smbus_write_byte_data
		(client, ICM20689REG_GYRO_CONFIG, 0x18);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_GYRO_CONFIG failed\r\n", __func__);
		return -1;
	}
	result =
		i2c_smbus_write_byte_data
		(client, ICM20689REG_ACCEL_CONFIG, 0x01);
	if (result < 0) {
		printk(KERN_INFO
			"==%s== REG_ACCEL_CONFIG failed\r\n", __func__);
		return -1;
	}

	return 0;
}

static int icm20689_measure(struct icm20689_report_s *pf)
{
	int result;
	uint8_t buff[14];
	struct timespec timestamp;

	result =
		i2c_smbus_read_i2c_block_data
		(icm20689_i2c_client, ICM20689REG_ACCEL_XOUT_H, 14, buff);
	if (result < 0) {
		printk(KERN_INFO "==%s== get OUT failed\r\n", __func__);
		return -1;
	}

	pf->acc[0] = buff[0] << 8 | buff[1];
	pf->acc[1] = buff[2] << 8 | buff[3];
	pf->acc[2] = buff[4] << 8 | buff[5];
	pf->temp = buff[6] << 8 | buff[7];
	pf->gyro[0] = buff[8] << 8 | buff[9];
	pf->gyro[1] = buff[10] << 8 | buff[11];
	pf->gyro[2] = buff[12] << 8 | buff[13];

	ktime_get_ts(&timestamp);
	pf->ts = timespec_to_ns(&timestamp);

	return 0;
}

int icm20689_open(struct inode *inode, struct file *filp)
{
	int ret;

	printk(KERN_INFO "==%s==\r\n", __func__);

	/*
		enable dldo3 for i2c pull
	*/
	vcc = regulator_get(NULL, "axp22_dldo3");
	ret = regulator_set_voltage(vcc, 3000000, 3000000);
	ret = regulator_enable(vcc);
	if (icm20689_i2c_test(icm20689_i2c_client) == false)
		return -1;

	icm20689_init_client(icm20689_i2c_client);
	return 0;
}

int icm20689_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "==%s==\r\n", __func__);

	regulator_disable(vcc);
	return 0;
}

static ssize_t icm20689_read(struct file *filp, char __user *buf, size_t size,
			    loff_t *ppos)
{
	struct icm20689_report_s data;
	int result;

	result = icm20689_measure(&data);
	if (result < 0)
		return -1;

	result = copy_to_user(buf, (void *)&data, sizeof(data));
	if (result < 0)
		return -1;

	return 1;
}

static const struct file_operations icm20689_fops = {
	.owner = THIS_MODULE,
	.read = icm20689_read,
	.open = icm20689_open,
	.release = icm20689_release,
};

static int icm20689_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	printk(KERN_INFO "==%s==\r\n", __func__);
	if (icm20689_i2c_client == NULL)
		icm20689_i2c_client = client;

	if (icm20689_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int icm20689_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id icm20689_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, icm20689_id);

static struct i2c_driver icm20689_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = icm20689_probe,
	.remove = icm20689_remove,
	.id_table = icm20689_id,
	.detect = icm20689_detect,
	.address_list = normal_i2c,
};

static int __init icm20689_init(void)
{
	int ret = -1;
	int result;

	printk(KERN_INFO "==%s==\r\n", __func__);

	ret = i2c_add_driver(&icm20689_driver);
	if (ret < 0) {
		printk(KERN_INFO "add icm20689 i2c driver failed\n");
		return -ENODEV;
	}

	result = alloc_chrdev_region(&devno, 0, 1, "icm20689");
	if (result < 0)
		return result;

	cdev = cdev_alloc();
	cdev_init(cdev, &icm20689_fops);
	cdev->owner = THIS_MODULE;
	result = cdev_add(cdev, devno, 1);

	cls = class_create(THIS_MODULE, "icm20689");
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		printk(KERN_INFO "==%s== class_create failed:%d\r\n", __func__,
		       ret);

	}

	device = device_create(cls, NULL, devno, NULL, "icm20689");
	if (IS_ERR(device)) {
		ret = PTR_ERR(device);
		printk(KERN_INFO "==%s== device_create failed:%d\r\n", __func__,
		       ret);
	}

	printk(KERN_INFO "==%s== done:%d\r\n", __func__, result);

	return ret;
}

static void __exit icm20689_exit(void)
{
	printk(KERN_INFO "remove icm20689 i2c driver.\n");
	device_destroy(cls, devno);
	class_destroy(cls);
	cdev_del(cdev);
	unregister_chrdev_region(devno, 1);
	i2c_del_driver(&icm20689_driver);
}

module_init(icm20689_init);
module_exit(icm20689_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
