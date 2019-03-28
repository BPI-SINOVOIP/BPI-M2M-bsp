/*
 * A V4L2 driver for isx012 cameras.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>

#include "camera.h"


MODULE_AUTHOR("lwj");
MODULE_DESCRIPTION("A low-level driver for isx012 sensors");
MODULE_LICENSE("GPL");

/* for internel driver debug */
#define DEV_DBG_EN      0
#if (DEV_DBG_EN == 1)
#define vfe_dev_dbg(x, arg...) printk(KERN_DEBUG"[isx012]"x, ##arg)
#else
#define vfe_dev_dbg(x, arg...)
#endif
#define vfe_dev_err(x, arg...) printk(KERN_ERR"[isx012]"x, ##arg)
#define vfe_dev_print(x, arg...) printk(KERN_INFO"[isx012]"x, ##arg)

#define LOG_ERR_RET(x)  { \
		int ret;  \
		ret = x; \
		if (ret < 0) {\
			vfe_dev_err("error at %s\n", __func__);  \
			return ret; \
		} \
	}

/* define module timing */
#define MCLK              (24*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_LOW
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR 0x0012
int isx012_sensor_vts;

/* define the voltage level of control signal*/
#define CSI_STBY_ON     0
#define CSI_STBY_OFF    1
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0
#define CSI_AF_PWR_ON   1
#define CSI_AF_PWR_OFF  0

#define regval_list		reg_list_w_a16_d16

#define REG_TERM 0xfffe
#define VAL_TERM 0xfe
#define REG_DLY  0xffff
#define CHANGE_I2C  0xfffd

/*
 * Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE 30

/*
 * The isx012 sits on i2c with ID 0x34
 */
#define I2C_ADDR 0x34
#define SENSOR_NAME "isx012_mipi"
static struct v4l2_subdev *glb_sd;

static struct cci_driver cci_drv = {
	.name = SENSOR_NAME,
};

/*
 * Information we maintain about a known sensor.
 */
struct sensor_format_struct;  /* coming later */

struct cfg_array { /* coming later */
	struct regval_list *regs;
	int size;
};

static inline struct sensor_info *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sensor_info, sd);
}


/*
 * The default register settings
 *
 */


static struct regval_list sensor_default_regs[] = {

{8, CHANGE_I2C, 0x1a},
{8, 0x0004, 0x02},
{8, 0x0007, 0x00},
{8, 0x0008, 0x00},
{8, 0x00C4, 0x11},
{8, 0x00C5, 0x11},
{8, 0x00C6, 0x11},
{8, 0x00C7, 0x11},
{8, 0x00C8, 0x11},
{8, 0x00C9, 0x11},
{8, 0x00CA, 0x11},
{8, 0x00CC, 0x11},
{8, 0x00CD, 0x11},
{8, 0x6A12, 0x50},
{8, 0x6A13, 0x50},
{8, 0x6A14, 0x50},
{8, 0x6A15, 0x50},
{16, 0x018C, 0x0000},
{16, 0x018E, 0x0000},
{16, 0x0190, 0x0000},
{16, 0x0192, 0x0000},
{16, 0x0194, 0x0000},
{16, 0x0196, 0x0000},
{16, 0x6A16, 0x0440},
{16, 0x6A18, 0x03C0},
{16, 0x6A1A, 0x01E0},
{16, 0x6A1C, 0x00E0},
{16, 0x6A1E, 0x0420},
{16, 0x6A20, 0x02C0},
{16, 0x0016, 0x0010},
{8, 0x5C01, 0x00},
{16, 0x0148, 0x0300},
{8, 0x0145, 0x01},
{8, 0x5C04, 0x04},
{8, 0x5C05, 0x03},
{8, 0x5C06, 0x0E},
{8, 0x5C07, 0x02},
{8, 0x5C08, 0x0B},
{8, 0x5C09, 0x05},
{8, 0x5C0A, 0x07},
{8, 0x5C0B, 0x03},
{8, 0x5C0C, 0x07},
{8, 0x5C0D, 0x05},
{8, 0x5C0E, 0x01},


{8, 0x00AD, 0x10},
{8, 0x00AC, 0x04},
{8, 0x0009, 0x01},
{8, 0x000a, 0x01},
{8, 0x00D0, 0x30},
{8, 0x00D1, 0x30},
{8, 0x00D2, 0x30},
{8, 0x00D3, 0x30},
{8, 0x00D4, 0x30},
{8, 0x00D5, 0x30},
{8, 0x00D6, 0x30},
{8, 0x00D7, 0x30},
{8, 0x00D8, 0x50},
{8, 0x00D9, 0x50},
{8, 0x00DA, 0x30},
{8, 0x00DB, 0x30},
{8, 0x0083, 0x01},
{8, 0x0084, 0x00},
{8, 0x0085, 0x00},
{8, 0x0086, 0x02},
{8, 0x0087, 0x03},
{8, 0x0088, 0x02},
{8, 0x0089, 0x0F},
{8, 0x008A, 0x0F},
{8, 0x008B, 0x0F},
{8, 0x00AF, 0x01},
{16, 0x0090, 0x0518},
{16, 0x0092, 0x0A30},
{16, 0x0094, 0x0A30},
{16, 0x0096, 0x03D4},
{16, 0x0098, 0x07A8},
{16, 0x009A, 0x0438},

{8, 0x0006, 0x16},

{8, CHANGE_I2C, 0x3c},
{8, REG_DLY, 0xff},

{8, 0x0140, 0x01},

};

/*for capture*/
static struct regval_list sensor_5M_regs[] = {
	{8, 0x0081, 0x02},
};

/*for video*/
static struct regval_list sensor_1080p_regs[] = {
	{8, 0x0081, 0x03},
};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */

static struct regval_list sensor_fmt_raw[] = {
};

/*
 * Low-level register I/O.
 *
 */


/*
 * On most platforms, we'd rather do straight i2c I/O.
 */
/*!!!!modified type for each device, be careful of the para type!!!*/
static int sensor_read_byte(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char *value)
{
	int ret = 0;
	int cnt = 0;

	ret = cci_read_a16_d8(sd, reg, value);
	while (ret != 0 && cnt < 2) {
		ret = cci_read_a16_d8(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor read retry=%d\n", cnt);

	return ret;
}
#if 0
static int sensor_read(struct v4l2_subdev *sd, unsigned short reg,
		unsigned short *value)
{
	int ret = 0;
	int cnt = 0;

	ret = cci_read_a16_d16(sd, reg, value);
	while (ret != 0 && cnt < 2) {
		ret = cci_read_a16_d16(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor read retry=%d\n", cnt);

	return ret;
}
#endif

static int sensor_write_byte(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char value)
{
	int ret = 0;
	int cnt = 0;

	if (CHANGE_I2C == reg) {
		cci_drv.cci_saddr = value;
		return 0;
	}
	ret = cci_write_a16_d8(sd, reg, value);
	while (ret != 0 && cnt < 2) {
		ret = cci_write_a16_d8(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor write 0x%x retry=%d\n", reg, cnt);

	return ret;
}

static int sensor_write(struct v4l2_subdev *sd, unsigned short reg,
		unsigned short value)
{
	int ret = 0;
	int cnt = 0;

	ret = cci_write_a16_d16(sd, reg, value);
	while (ret != 0 && cnt < 2) {
		ret = cci_write_a16_d16(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor write 0x%x retry=%d\n", reg, cnt);

	return ret;
}

/*
 * Write a list of register settings;
 */
static int sensor_write_array(struct v4l2_subdev *sd,
		struct regval_list *regs, int array_size)
{
	int i = 0;

	if (!regs)
		return -EINVAL;

	while (i < array_size) {
		if (regs->addr == REG_DLY)
			msleep(regs->data);
		else {
			if (regs->width == 16)
				LOG_ERR_RET(sensor_write(sd,
					regs->addr, regs->data))
			else if (regs->width == 8)
				LOG_ERR_RET(sensor_write_byte(sd,
					regs->addr, regs->data))
		}
		i++;
		regs++;
	}
	return 0;
}

/*
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function ,retrun -EINVAL
 */
static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->exp;
	vfe_dev_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}
static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	struct sensor_info *info = to_state(sd);

	if (info->exp == exp_val)
		return 0;

	sensor_write(sd, 0x0148, exp_val>>4);

	info->exp = exp_val;
	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->gain;
	vfe_dev_dbg("sensor_get_gain = %d\n", info->gain);
	return 0;
}

unsigned char gain2db[497] = {
	0, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 23,
	24, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 32, 33, 34, 34,
	35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40,
	40, 41, 41, 41, 42, 42, 43, 43, 44, 44, 44, 45, 45, 45, 46, 46,
	47, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50,
	50, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 54, 54, 54, 54, 55,
	55, 55, 56, 56, 56, 56, 57, 57, 57, 57, 58,
	58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 60, 60, 61, 61, 61, 61,
	62, 62, 62, 62, 62, 63, 63, 63, 63, 63, 64,
	64, 64, 64, 64, 65, 65, 65, 65, 65, 66, 66, 66, 66, 66, 66, 67,
	67, 67, 67, 67, 68, 68, 68, 68, 68, 68, 69,
	69, 69, 69, 69, 69, 70, 70, 70, 70, 70, 70, 71, 71, 71, 71, 71,
	71, 71, 72, 72, 72, 72, 72, 72, 73, 73, 73,
	73, 73, 73, 73, 74, 74, 74, 74, 74, 74, 74, 75, 75, 75, 75, 75,
	75, 75, 75, 76, 76, 76, 76, 76, 76, 76, 77,
	77, 77, 77, 77, 77, 77, 77, 78, 78, 78, 78, 78, 78, 78, 78, 79,
	79, 79, 79, 79, 79, 79, 79, 79, 80, 80, 80,
	80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 81, 81, 81, 82,
	82, 82, 82, 82, 82, 82, 82, 82, 83, 83, 83,
	83, 83, 83, 83, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 84, 84,
	84, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
	86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87,
	87, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88,
	88, 88, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 89,
	89, 89, 89, 90, 90, 90, 90, 90, 90, 90, 90,
	90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
	91, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
	92, 92, 92, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
	93, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
	94, 94, 94, 94, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95,
	95, 95, 95, 96, 96, 96, 96, 96, 96, 96, 96,
	96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97, 97, 97,
	97, 97, 97, 97, 97, 97, 97, 98, 98, 98, 98,
	98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100
};

static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);

	sensor_write_byte(sd, 0x014a, gain2db[gain_val-16]);

	info->gain = gain_val;
	return 0;
}


static int sensor_s_exp_gain(struct v4l2_subdev *sd,
			struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val;
	struct sensor_info *info = to_state(sd);
	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if (gain_val < 1*16)
		gain_val = 16;
	if (gain_val > 64*16-1)
		gain_val = 64*16-1;

	if (exp_val > 0xfffff)
		exp_val = 0xfffff;

	vfe_dev_print("isx012 sensor_set_gain = %d, %d  Done!\n",
			gain_val, exp_val);

	sensor_s_gain(sd, gain_val);
	sensor_s_exp(sd, exp_val);

	info->gain = gain_val;
	info->exp = exp_val;
	return 0;
}

#if 0
static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	unsigned char rdval;

	ret = sensor_read_byte(sd, 0x0100, &rdval);
	if (ret != 0)
		return ret;

	if (on_off == CSI_STBY_ON)
		ret = sensor_write_byte(sd, 0x0100, rdval&0xfe);
	else
		ret = sensor_write_byte(sd, 0x0100, rdval|0x01);

	return ret;
}
#endif
/*
 * Stuff that knows about the sensor.
 */

static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;
	ret = 0;
	switch (on) {
	case CSI_SUBDEV_PWR_ON:
	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_print("CSI_SUBDEV_PWR_ON!\n");
		cci_lock(sd);
		vfe_gpio_write(sd, POWER_EN, CSI_PWR_ON);
		vfe_set_pmu_channel(sd, DVDD, ON);
		vfe_set_pmu_channel(sd, IOVDD, ON);
		vfe_set_pmu_channel(sd, AVDD, ON);
		vfe_set_pmu_channel(sd, AFVDD, ON);
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		vfe_gpio_set_status(sd, PWDN, 1);
		vfe_gpio_set_status(sd, RESET, 1);
		vfe_gpio_write(sd, RESET, CSI_RST_OFF);
		usleep_range(30000, 31000);
		vfe_gpio_write(sd, PWDN, CSI_STBY_OFF);
		msleep(50);
		usleep_range(10000, 12000);
		cci_unlock(sd);
		break;
	case CSI_SUBDEV_PWR_OFF:
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_print("CSI_SUBDEV_PWR_OFF!\n");
		cci_lock(sd);
		usleep_range(10000, 12000);
		vfe_gpio_write(sd, PWDN, CSI_STBY_ON);
		msleep(20);
		vfe_gpio_write(sd, RESET, CSI_RST_ON);
		vfe_gpio_set_status(sd, RESET, 0);
		vfe_gpio_set_status(sd, PWDN, 0);
		vfe_set_mclk(sd, OFF);
		vfe_gpio_write(sd, POWER_EN, CSI_PWR_OFF);
		vfe_set_pmu_channel(sd,  AFVDD, OFF);
		vfe_set_pmu_channel(sd, DVDD, OFF);
		vfe_set_pmu_channel(sd, AVDD, OFF);
		vfe_set_pmu_channel(sd, IOVDD, OFF);
		msleep(20);
		cci_unlock(sd);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
	switch (val) {
	case 0:
		vfe_gpio_write(sd, RESET, CSI_RST_OFF);
		usleep_range(10000, 12000);
		break;
	case 1:
		vfe_gpio_write(sd, RESET, CSI_RST_ON);
		usleep_range(10000, 12000);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	unsigned char rdval;
	cci_drv.cci_saddr = 0x1a;

	sensor_read_byte(sd, 0x0000, &rdval);
	vfe_dev_print("isx012 sensor = %x\n", rdval);

	return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_init\n");

	/*Make sure it is a target sensor*/
	ret = sensor_detect(sd);
	if (ret) {
		vfe_dev_err("chip found is not an target chip.\n");
		return ret;
	}
	vfe_get_standby_mode(sd, &info->stby_mode);

	if ((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY) \
		&& info->init_first_flag == 0) {
		vfe_dev_print("stby_mode and init_first_flag = 0\n");
		return 0;
	}

	info->focus_status = 0;
	info->low_speed = 0;
	info->width = 1920;
	info->height = 1080;
	info->hflip = 0;
	info->vflip = 0;
	info->gain = 0;
	info->exp = 0;
	info->gain = 0;

	info->tpf.numerator = 1;
	info->tpf.denominator = 30;    /* 30fps */

	ret = sensor_write_array(sd, sensor_default_regs,
			ARRAY_SIZE(sensor_default_regs));
	if (ret < 0) {
		vfe_dev_err("write sensor_default_regs error\n");
		return ret;
	}
	if (info->stby_mode == 0)
		info->init_first_flag = 0;
	info->preview_first_flag = 1;
	return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct sensor_info *info = to_state(sd);
	switch (cmd) {
	case GET_CURRENT_WIN_CFG:
		if (info->current_wins != NULL) {
			memcpy(arg, info->current_wins,
				sizeof(struct sensor_win_size));
			ret = 0;
		} else {
			vfe_dev_err("empty wins!\n");
			ret = -1;
		}
		break;
	case SET_FPS:
		ret = 0;
		break;
	case ISP_SET_EXP_GAIN:
		ret = sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	default:
		return -EINVAL;
	}
	return ret;
}


/*
 * Store information about the video data format.
 */
static struct sensor_format_struct {
	__u8 *desc;
	enum v4l2_mbus_pixelcode mbus_code;
	struct regval_list *regs;
	int regs_size;
	int bpp;   /* Bytes per pixel */
} sensor_formats[] = {
	{
		.desc		= "Raw RGB Bayer",
		.mbus_code	= V4L2_MBUS_FMT_SRGGB10_10X1,
		.regs		= sensor_fmt_raw,
		.regs_size  = ARRAY_SIZE(sensor_fmt_raw),
		.bpp		= 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */
static struct sensor_win_size sensor_win_sizes[] = {
	/* 5M */
	{
		.width      = 2608,
		.height     = 1960,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 2784,
		.vts        = 2030,
		.pclk       = 86400000,
		.mipi_bps	  = 648*1000*1000,
		.fps_fixed  = 15,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2030-4)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs       = sensor_5M_regs,
		.regs_size  = ARRAY_SIZE(sensor_5M_regs),
		.set_size   = NULL,
},

	{
		.width      = 1920,
		.height     = 1440,
		.hoffset    = 344,
		.voffset    = 260,
		.hts        = 2784,
		.vts        = 2030,
		.pclk       = 86400000,
		.mipi_bps	  = 648*1000*1000,
		.fps_fixed  = 15,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2030-4)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs       = sensor_5M_regs,
		.regs_size  = ARRAY_SIZE(sensor_5M_regs),
		.set_size   = NULL,
	},

	{
		.width      = 1728,
		.height     = 1296,
		.hoffset    = 440,
		.voffset    = 332,
		.hts        = 2784,
		.vts        = 2030,
		.pclk       = 86400000,
		.mipi_bps	  = 648*1000*1000,
		.fps_fixed  = 15,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2030-4)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs       = sensor_5M_regs,
		.regs_size  = ARRAY_SIZE(sensor_5M_regs),
		.set_size   = NULL,
	},

	{
		.width      = 1920,
		.height     = 1080,
		.hoffset    = 344,
		.voffset    = 0,
		.hts        = 2500,
		.vts        = 1120,
		.pclk       = 84000000,
		.mipi_bps	  = 648*1000*1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2030-4)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs       = sensor_1080p_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p_regs),
		.set_size   = NULL,
},

	{
		.width      = 1280,
		.height     = 720,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 2500,
		.vts        = 1120,
		.pclk       = 84000000,
		.mipi_bps	  = 648*1000*1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2030-4)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.width_input	  = 1920,
		.height_input	  = 1080,
		.regs       = sensor_1080p_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p_regs),
		.set_size   = NULL,
	},
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_enum_fmt(struct v4l2_subdev *sd, unsigned index,
		enum v4l2_mbus_pixelcode *code)
{
	if (index >= N_FMTS)
		return -EINVAL;

	*code = sensor_formats[index].mbus_code;
	return 0;
}

static int sensor_enum_size(struct v4l2_subdev *sd,
			struct v4l2_frmsizeenum *fsize)
{
	if (fsize->index > N_WIN_SIZES-1)
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = sensor_win_sizes[fsize->index].width;
	fsize->discrete.height = sensor_win_sizes[fsize->index].height;
	fsize->reserved[0] = sensor_win_sizes[fsize->index].fps_fixed;
	return 0;
}

static int sensor_try_fmt_internal(struct v4l2_subdev *sd,
	struct v4l2_mbus_framefmt *fmt,
	struct sensor_format_struct **ret_fmt,
	struct sensor_win_size **ret_wsize)
{
	int index;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	for (index = 0; index < N_FMTS; index++)
		if (sensor_formats[index].mbus_code == fmt->code)
			break;

	if (index >= N_FMTS)
		return -EINVAL;

	if (ret_fmt != NULL)
		*ret_fmt = sensor_formats + index;

	/*
	* Fields: the sensor devices claim to be progressive.
	*/
	fmt->field = V4L2_FIELD_NONE;

	/*
	* Round requested image size down to the nearest
	* we support, but not below the smallest.
	*/
	for (wsize = sensor_win_sizes; wsize < sensor_win_sizes + N_WIN_SIZES;
			wsize++)
		if (fmt->width >= wsize->width && fmt->height >= wsize->height
				&& info->tpf.denominator == wsize->fps_fixed)
			break;

	if (wsize >= sensor_win_sizes + N_WIN_SIZES)
		wsize--;   /* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;
	/*
	* Note the size we'll actually handle.
	*/
	fmt->width = wsize->width;
	fmt->height = wsize->height;
	info->current_wins = wsize;

	return 0;
}

static int sensor_try_fmt(struct v4l2_subdev *sd,
		struct v4l2_mbus_framefmt *fmt)
{
	return sensor_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
		struct v4l2_mbus_config *cfg)
{
	cfg->type = V4L2_MBUS_CSI2;
	cfg->flags = 0|V4L2_MBUS_CSI2_2_LANE|V4L2_MBUS_CSI2_CHANNEL_0;
	return 0;
}


/*
 * Set a format.
 */
static int sensor_s_fmt(struct v4l2_subdev *sd,
		struct v4l2_mbus_framefmt *fmt)
{
	int ret;
	struct sensor_format_struct *sensor_fmt;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_s_fmt\n");
	ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
	if (ret)
		return ret;

	LOG_ERR_RET(sensor_write_array(sd,
		sensor_fmt->regs, sensor_fmt->regs_size))
	ret = 0;
	if (wsize->regs)
		LOG_ERR_RET(sensor_write_array(sd,
			wsize->regs, wsize->regs_size))
	if (wsize->set_size)
		LOG_ERR_RET(wsize->set_size(sd))

	info->fmt = sensor_fmt;
	info->width = wsize->width;
	info->height = wsize->height;
	info->exp = 0;
	info->gain = 0;
	isx012_sensor_vts = wsize->vts;

	vfe_dev_print("s_fmt = %x, width = %d, height = %d\n",
		sensor_fmt->mbus_code, wsize->width, wsize->height);

	return 0;
}

/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->capturemode = info->capture_mode;
	cp->timeperframe = info->tpf;
	return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_s_parm\n");

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	info->tpf = cp->timeperframe;
	vfe_dev_dbg("isx012_s_parm fps = %d\n", info->tpf.denominator);
	if (info->tpf.numerator == 0)
		return -EINVAL;

	info->capture_mode = cp->capturemode;

	return 0;
}


static int sensor_queryctrl(struct v4l2_subdev *sd,
		struct v4l2_queryctrl *qc)
{
	/* Fill in min, max, step and default value for these controls. */
	/* see include/linux/videodev2.h for details */
	switch (qc->id) {
	case V4L2_CID_GAIN:
		return v4l2_ctrl_query_fill(qc, 1*16, 128*16-1, 1, 16);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 0, 65536*16, 1, 0);
	}
	return -EINVAL;
}

static int sensor_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_g_gain(sd, &ctrl->value);
	case V4L2_CID_EXPOSURE:
		return sensor_g_exp(sd, &ctrl->value);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	struct v4l2_queryctrl qc;
	int ret;

	qc.id = ctrl->id;
	ret = sensor_queryctrl(sd, &qc);
	if (ret < 0)
		return ret;

	if (ctrl->value < qc.minimum || ctrl->value > qc.maximum)
		return -ERANGE;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->value);
	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->value);
	}
	return -EINVAL;
}


static int sensor_g_chip_ident(struct v4l2_subdev *sd,
	struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_SENSOR, 0);
}


/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops sensor_core_ops = {
	.g_chip_ident = sensor_g_chip_ident,
	.g_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
	.queryctrl = sensor_queryctrl,
	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.enum_mbus_fmt = sensor_enum_fmt,
	.enum_framesizes = sensor_enum_size,
	.try_mbus_fmt = sensor_try_fmt,
	.s_mbus_fmt = sensor_s_fmt,
	.s_parm = sensor_s_parm,
	.g_parm = sensor_g_parm,
	.g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
	.video = &sensor_video_ops,
};

/* ----------------------------------------------------------------------- */

static int sensor_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct v4l2_subdev *sd;
	struct sensor_info *info;

	info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	sd = &info->sd;
	glb_sd = sd;
	cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);

	info->fmt = &sensor_formats[0];
	info->af_first_flag = 1;
	info->init_first_flag = 1;
	info->exp = 0;
	info->gain = 0;

	return 0;
}


static int sensor_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd;
	sd = cci_dev_remove_helper(client, &cci_drv);
	kfree(to_state(sd));
	return 0;
}

static const struct i2c_device_id sensor_id[] = {
	{ SENSOR_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, sensor_id);


static struct i2c_driver sensor_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SENSOR_NAME,
	},
	.probe = sensor_probe,
	.remove = sensor_remove,
	.id_table = sensor_id,
};
static __init int init_sensor(void)
{
	return cci_dev_init_helper(&sensor_driver);
}

static __exit void exit_sensor(void)
{
	cci_dev_exit_helper(&sensor_driver);
}

module_init(init_sensor);
module_exit(exit_sensor);

