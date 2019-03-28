/*
 * A V4L2 driver for IMX208 Raw cameras.
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

#include "../isp_cfg/SENSOR_H/imx208_default_v3.h"

#include "camera.h"


MODULE_AUTHOR("lwj");
MODULE_DESCRIPTION("A low-level driver for IMX208 sensors");
MODULE_LICENSE("GPL");

/* for internel driver debug */
#define DEV_DBG_EN      0
#if (DEV_DBG_EN == 1)
#define vfe_dev_dbg(x, arg...) printk(KERN_DEBUG"[IMX208]"x, ##arg)
#else
#define vfe_dev_dbg(x, arg...)
#endif
#define vfe_dev_err(x, arg...) printk(KERN_ERR"[IMX208]"x, ##arg)
#define vfe_dev_print(x, arg...) printk(KERN_INFO"[IMX208]"x, ##arg)

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
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_HIGH
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR  0x0208


/* define the voltage level of control signal*/
#define CSI_STBY_ON     0
#define CSI_STBY_OFF    1
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0
#define CSI_AF_PWR_ON   1
#define CSI_AF_PWR_OFF  0
#define regval_list reg_list_a16_d8


/* define the registers */
#define REG_TERM 0xfffe
#define VAL_TERM 0xfe
#define REG_DLY  0xffff

#define ID_REG_HIGH		0x0000
#define ID_REG_LOW		0x0001
#define ID_VAL_HIGH		((V4L2_IDENT_SENSOR) >> 8)
#define ID_VAL_LOW		((V4L2_IDENT_SENSOR) & 0xff)

/*
 *Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE 30


/*
 * The IMX208 i2c address
 */
#define I2C_ADDR 0x34



static struct v4l2_subdev *glb_sd;
#define SENSOR_NAME "imx208"

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
	{0x0103, 0x01},
	{0xffff, 0x10},

};

static struct regval_list sensor_1080p60_regs[] = {
	{0x0100, 0x00},
	{0xffff, 0x10},
	{0x0305, 0x04},
	{0x0307, 0x87},
	{0x303C, 0x4B},
	{0x30A4, 0x02},

	{0x0112, 0x0A},
	{0x0113, 0x0A},
	{0x0340, 0x04},
	{0x0341, 0xB0},
	{0x0342, 0x04},
	{0x0343, 0x64},
	{0x0344, 0x00},
	{0x0345, 0x00},
	{0x0346, 0x00},
	{0x0347, 0x00},
	{0x0348, 0x07},
	{0x0349, 0x8F},
	{0x034A, 0x04},
	{0x034B, 0x47},
	{0x034C, 0x07},
	{0x034D, 0x90},
	{0x034E, 0x04},
	{0x034F, 0x48},
	{0x0381, 0x01},
	{0x0383, 0x01},
	{0x0385, 0x01},
	{0x0387, 0x01},
	{0x3048, 0x00},
	{0x304E, 0x0A},
	{0x3050, 0x01},
	{0x309B, 0x00},
	{0x30D5, 0x00},
	{0x3301, 0x00},
	{0x3318, 0x61},

	{0x0202, 0x01},
	{0x0203, 0x90},
	{0x0205, 0x00},
	{0x0100, 0x01},
};

static struct regval_list sensor_1080p30_regs[] = {
	{0x0100, 0x00},
	{0xffff, 0x10},
	{0x0305, 0x04},
	{0x0307, 0x87},
	{0x303C, 0x4B},
	{0x30A4, 0x02},

	{0x0112, 0x0A},
	{0x0113, 0x0A},
	{0x0340, 0x09},
	{0x0341, 0x60},
	{0x0342, 0x04},
	{0x0343, 0x64},
	{0x0344, 0x00},
	{0x0345, 0x00},
	{0x0346, 0x00},
	{0x0347, 0x00},
	{0x0348, 0x07},
	{0x0349, 0x8F},
	{0x034A, 0x04},
	{0x034B, 0x47},
	{0x034C, 0x07},
	{0x034D, 0x90},
	{0x034E, 0x04},
	{0x034F, 0x48},
	{0x0381, 0x01},
	{0x0383, 0x01},
	{0x0385, 0x01},
	{0x0387, 0x01},
	{0x3048, 0x00},
	{0x304E, 0x0A},
	{0x3050, 0x01},
	{0x309B, 0x00},
	{0x30D5, 0x00},
	{0x3301, 0x00},
	{0x3318, 0x61},

	{0x0202, 0x01},
	{0x0203, 0x90},
	{0x0205, 0x00},
	{0x0100, 0x01},
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
static int sensor_read(struct v4l2_subdev *sd, unsigned short reg,
			unsigned char *value)
{
	int ret = 0;
	int cnt = 0;

	ret = cci_read_a16_d8(sd, reg, value);
	while (ret != 0 && cnt < 3) {
		ret = cci_read_a16_d8(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor read retry=%d\n", cnt);

	return ret;
}

static int sensor_write(struct v4l2_subdev *sd, unsigned short reg,
			unsigned char value)
{
	int ret = 0;
	int cnt = 0;

	ret = cci_write_a16_d8(sd, reg, value);
	while (ret != 0 && cnt < 2) {
		ret = cci_write_a16_d8(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor write retry=%d\n", cnt);

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
		else
			LOG_ERR_RET(sensor_write(sd, regs->addr, regs->data))
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
	unsigned char explow, exphigh;
	struct sensor_info *info = to_state(sd);

	if (exp_val > 0xffffff)
		exp_val = 0xfffff0;
	if (exp_val < 16)
		exp_val = 16;

	exp_val = (exp_val + 8) >> 4;	/*rounding to 1*/

	exphigh = (unsigned char)((0xff00 & exp_val) >> 8);
	explow = (unsigned char)((0x00ff & exp_val));

	sensor_write(sd, 0x0203, explow);	/*coarse integration time*/
	sensor_write(sd, 0x0202, exphigh);

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

static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);
	unsigned char gainlow = 0;
	unsigned char gainhigh = 0;
	unsigned char gain_digi = 0;
	int gain_ana = 0;
	int gain_sensor = 0;

	if (gain_val <= 128) {
		gain_ana = gain_val;
		gain_digi = 1;
	} else if (gain_val <= 256) {
		gain_ana = gain_val >> 1;
		gain_digi = 2;
	} else {
		gain_ana = gain_val >> 2;
		gain_digi = 4;
	}

	gain_sensor = 256 - 4096 / gain_ana;

	gainlow = (unsigned char)(gain_sensor & 0xff);
	gainhigh = (unsigned char)((gain_sensor >> 8) & 0xff);

	sensor_write(sd, 0x0205, gainlow);
	sensor_write(sd, 0x0204, gainhigh);

	sensor_write(sd, 0x020E, gain_digi);
	sensor_write(sd, 0x0210, gain_digi);
	sensor_write(sd, 0x0212, gain_digi);
	sensor_write(sd, 0x0214, gain_digi);

	info->gain = gain_val;
	return 0;
}

static int IMX208_sensor_vts;
static int sensor_s_exp_gain(struct v4l2_subdev *sd,
		struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val, shutter, frame_length;
	struct sensor_info *info = to_state(sd);
	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if (gain_val < 1*16)
		gain_val = 16;
	if (gain_val > 64*16-1)
		gain_val = 64*16-1;
	if (exp_val > 0xfffff)
		exp_val = 0xfffff;
	shutter = exp_val / 16;
	if (shutter  > IMX208_sensor_vts - 5)
		frame_length = shutter + 5;
	else
		frame_length = IMX208_sensor_vts;

	sensor_write(sd, 0x0104, 0x01);
	sensor_write(sd, 0x0341, (frame_length & 0xff));
	sensor_write(sd, 0x0340, (frame_length >> 8));
	sensor_s_exp(sd, exp_val);
	sensor_s_gain(sd, gain_val);
	sensor_write(sd, 0x0104, 0x00);

	info->exp = exp_val;
	info->gain = gain_val;
	return 0;
}


static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	unsigned char rdval;

	ret = sensor_read(sd, 0x0100, &rdval);
	if (ret != 0)
		return ret;

	if (on_off == CSI_GPIO_HIGH)	/*sw stby on*/
		ret = sensor_write(sd, 0x0100, rdval & 0xfe);
	else			/*sw stby off*/
		ret = sensor_write(sd, 0x0100, rdval | 0x01);
	return ret;
}


/*
 * Stuff that knows about the sensor.
 */
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;

	ret = 0;
	switch (on) {
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
		ret = sensor_s_sw_stby(sd, CSI_GPIO_HIGH);
		if (ret < 0)
			vfe_dev_err("soft stby falied!\n");
		usleep_range(10000, 12000);
		cci_lock(sd);
		cci_unlock(sd);
		vfe_set_mclk(sd, OFF);
		break;
	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
		cci_lock(sd);
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		usleep_range(10000, 12000);
		ret = sensor_s_sw_stby(sd, CSI_GPIO_LOW);
		if (ret < 0)
			vfe_dev_err("soft stby off falied!\n");
		cci_unlock(sd);
		break;
	case CSI_SUBDEV_PWR_ON:
		vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
		cci_lock(sd);
		vfe_gpio_set_status(sd, PWDN, 1);
		vfe_gpio_set_status(sd, RESET, 1);
		vfe_gpio_set_status(sd, POWER_EN, 1);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);

		vfe_gpio_write(sd, POWER_EN, CSI_PWR_ON);
		vfe_set_pmu_channel(sd, IOVDD, ON);
		vfe_set_pmu_channel(sd, AVDD, ON);
		vfe_set_pmu_channel(sd, DVDD, ON);
		usleep_range(7000, 8000);

		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);

		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);

		cci_unlock(sd);
		break;
	case CSI_SUBDEV_PWR_OFF:
		vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");
		cci_lock(sd);
		vfe_gpio_set_status(sd, PWDN, 1);
		vfe_gpio_set_status(sd, RESET, 1);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		vfe_set_mclk(sd, OFF);
		vfe_set_pmu_channel(sd, AFVDD, OFF);
		vfe_set_pmu_channel(sd, AVDD, OFF);
		vfe_set_pmu_channel(sd, IOVDD, OFF);
		vfe_set_pmu_channel(sd, DVDD, OFF);
		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		vfe_gpio_set_status(sd, RESET, 0);
		vfe_gpio_set_status(sd, PWDN, 0);
		vfe_gpio_set_status(sd, POWER_EN, 0);
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

	LOG_ERR_RET(sensor_read(sd, ID_REG_HIGH, &rdval))
	vfe_dev_dbg("IMX208 ID_VAL_HIGH = %2x, Done!\n", rdval);
	if (rdval != ID_VAL_HIGH)
		return -ENODEV;

	LOG_ERR_RET(sensor_read(sd, ID_REG_LOW, &rdval))
	vfe_dev_dbg("IMX208 ID_VAL_LOW = %2x, Done!\n", rdval);
	if (rdval != ID_VAL_LOW)
		return -ENODEV;

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
	info->width = HD1080_WIDTH;
	info->height = HD1080_HEIGHT;
	info->hflip = 0;
	info->vflip = 0;
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
	/* 1080P */
	{
		.width		= HD1080_WIDTH,
		.height		= HD1080_HEIGHT,
		.hoffset	= 0,
		.voffset	= 0,
		.hts		= 2250,
		.vts		= 1200,
		.pclk		= 162*1000*1000,
		.mipi_bps	= 810*1000*1000,
		.fps_fixed	= 60,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (1200-5)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs       = sensor_1080p60_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p60_regs),
		.set_size	  = NULL,
		.sensor_ae_tbl = {
			.ae_tbl = {
				{8000, 60,  256   , 256, },
				{60  , 60,  256   , 8000, },
			},
			.length = 2,
			.ev_step = 40,
		},
		.isp_cfg = &imx208_default_v3,
	},

	{
		.width		= HD1080_WIDTH,
		.height		= HD1080_HEIGHT,
		.hoffset	= 0,
		.voffset	= 0,
		.hts        = 2250,
		.vts        = 2400,
		.pclk       = 162*1000*1000,
		.mipi_bps	  = 810*1000*1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2400-5)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.regs		  = sensor_1080p30_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p30_regs),
		.set_size	  = NULL,
		.sensor_ae_tbl = {
			.ae_tbl = {
				{8000, 30,  256   , 256, },
				{30  , 30,  256   , 4000, },
			},
			.length = 2,
			.ev_step = 40,
		},
		.isp_cfg = &imx208_default_v3,
	},

	/* 720p */
	{
		.width		= 1280,
		.height		= 720,
		.hoffset	= 0,
		.voffset	= 0,
		.hts        = 2250,
		.vts        = 1200,
		.pclk       = 162*1000*1000,
		.mipi_bps	  = 810*1000*1000,
		.fps_fixed  = 60,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (1200-5)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.width_input	   = 1920,
		.height_input	   = 1080,
		.regs       = sensor_1080p60_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p60_regs),
		.set_size	  = NULL,
		.sensor_ae_tbl = {
			.ae_tbl = {
				{8000, 60,  256   , 256, },
				{60  , 60,  256   , 8000, },
			},
			.length = 2,
			.ev_step = 40,
		},
		.isp_cfg = &imx208_default_v3,
	},

	{
		.width		= 1280,
		.height		= 720,
		.hoffset	= 0,
		.voffset	= 0,
		.hts        = 2250,
		.vts        = 2400,
		.pclk       = 162*1000*1000,
		.mipi_bps	  = 810*1000*1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1<<4,
		.intg_max   = (2400-5)<<4,
		.gain_min   = 1<<4,
		.gain_max   = 32<<4,
		.width_input	   = 1920,
		.height_input	   = 1080,
		.regs		  = sensor_1080p30_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p30_regs),
		.set_size	  = NULL,
		.sensor_ae_tbl = {
			.ae_tbl = {
				{8000, 30,  256   , 256, },
				{30  , 30,  256   , 4000, },
			},
			.length = 2,
			.ev_step = 40,
		},
		.isp_cfg = &imx208_default_v3,
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
	IMX208_sensor_vts = wsize->vts;

	vfe_dev_print("s_fmt set width = %d, height = %d\n",
		wsize->width, wsize->height);
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
		return v4l2_ctrl_query_fill(qc, 1, 65536*16, 1, 1);
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
static struct cci_driver cci_drv = {
	.name = SENSOR_NAME,
};

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

