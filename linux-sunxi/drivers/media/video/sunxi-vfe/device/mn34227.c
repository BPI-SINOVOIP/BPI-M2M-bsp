/*
 * A V4L2 driver for MN34227 cameras.
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
MODULE_DESCRIPTION("A low-level driver for MN34227 sensors");
MODULE_LICENSE("GPL");

/* for internel driver debug */
#define DEV_DBG_EN      1
#if (DEV_DBG_EN == 1)
#define vfe_dev_dbg(x, arg...) printk(KERN_DEBUG"[MN34227]"x, ##arg)
#else
#define vfe_dev_dbg(x, arg...)
#endif
#define vfe_dev_err(x, arg...) printk(KERN_ERR"[MN34227]"x, ##arg)
#define vfe_dev_print(x, arg...) printk(KERN_INFO"[MN34227]"x, ##arg)

#define LOG_ERR_RET(x)  { \
			int ret;  \
			ret = x; \
			if (ret < 0) {\
				vfe_dev_err("error at %s\n", __func__);  \
				return ret; \
			} \
		}

/* define module timing */
#define MCLK              (27*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_HIGH
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR  0x0291

/* define the voltage level of control signal */
#define CSI_STBY_ON     1
#define CSI_STBY_OFF    0
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0
#define CSI_AF_PWR_ON   1
#define CSI_AF_PWR_OFF  0
#define regval_list reg_list_a16_d8

#define REG_TERM 0xfffe
#define VAL_TERM 0xfe
#define REG_DLY  0xffff

/* 1125 for 27Mhz */
#define VMAX 1125

/*
 * Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE 30

/*
 * The MN34227 i2c address
 */
#define I2C_ADDR 0x6c


static struct v4l2_subdev *glb_sd;
#define SENSOR_NAME "mn34227"

/*
 * Information we maintain about a known sensor.
 */
struct sensor_format_struct;	/* coming later */

struct cfg_array {		/* coming later */
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
	{0x300E, 0x01},
	{0x300F, 0x00},
	{0x0305, 0x01},
	{0x0307, 0x21},
	{0x3000, 0x00},
	{0x3001, 0x03},
	{0x0112, 0x0C},
	{0x0113, 0x0C},
	{0x3005, 0x64},
	{0x3007, 0x14},
	{0x3008, 0x91},
	{0x3009, 0x0E},
	{0x300B, 0x00},
	{0x3018, 0x43},
	{0x3019, 0x10},
	{0x301A, 0xB9},
	{0x3000, 0x00},
	{0x3001, 0x53},
	{0x300E, 0x00},
	{0x300F, 0x00},
	{0x0202, 0x04},
	{0x0203, 0x63},
	{0x3036, 0x00},
	{0x3039, 0x2E},
	{0x3058, 0x0F},
	{0x3059, 0xFF},
	{0x305B, 0x00},
	{0x3062, 0x10},
	{0x3063, 0x24},
	{0x306E, 0x0C},
	{0x306F, 0x00},
	{0x3074, 0x01},
	{0x3076, 0x40},
	{0x307C, 0x2C},
	{0x3085, 0x33},
	{0x3087, 0x01},
	{0x3088, 0x07},
	{0x3089, 0x02},
	{0x308B, 0x09},
	{0x308C, 0x03},
	{0x308D, 0x02},
	{0x308E, 0x02},
	{0x308F, 0x02},
	{0x3090, 0x02},
	{0x3094, 0x0B},
	{0x3095, 0x76},
	{0x3098, 0x00},
	{0x3099, 0x00},
	{0x309A, 0x01},
	{0x3104, 0x04},
	{0x3106, 0x00},
	{0x3107, 0xC0},
	{0x3141, 0x40},
	{0x3143, 0x02},
	{0x3144, 0x02},
	{0x3145, 0x02},
	{0x3146, 0x00},
	{0x3147, 0x02},
	{0x314A, 0x01},
	{0x314B, 0x02},
	{0x314C, 0x02},
	{0x314D, 0x02},
	{0x314E, 0x01},
	{0x314F, 0x02},
	{0x3150, 0x02},
	{0x3152, 0x04},
	{0x3153, 0xE3},
	{0x316F, 0xC6},
	{0x3175, 0x80},
	{0x318E, 0x20},
	{0x318F, 0x70},
	{0x3196, 0x08},
	{0x3211, 0x14},
	{0x323A, 0x80},
	{0x323B, 0x91},
	{0x323D, 0x90},
	{0x3243, 0xD7},
	{0x3247, 0x38},
	{0x3248, 0x03},
	{0x3249, 0xE2},
	{0x324A, 0x30},
	{0x324B, 0x18},
	{0x324C, 0x02},
	{0x3253, 0xDE},
	{0x3259, 0x68},
	{0x3272, 0x46},
	{0x3280, 0x30},
	{0x3288, 0x01},
	{0x330E, 0x05},
	{0x3310, 0x02},
	{0x3315, 0x1F},
	{0x332C, 0x02},
	{0x3339, 0x02},
	{0x3000, 0x00},
	{0x3001, 0xD3},
	{0x0100, 0x01},
	{0x0101, 0x00},

};

static struct regval_list sensor_1080p30_regs[] = {

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
	while (ret != 0 && cnt < 2) {
		ret = cci_read_a16_d8(sd, reg, value);
		cnt++;
	}
	if (cnt > 0)
		vfe_dev_dbg("sensor read retry=%d\n", cnt);

	return ret;
}



/*
static int show_regs_array (struct v4l2_subdev *sd,struct regval_list *array)
{	int i =0;
	unsigned char tmp;
	for(i=0;i<(sizeof(sensor_1080p_regs)/3);i++) {
		if((array+i)->addr == REG_DLY)
			msleep((array+i)->data);

		sensor_read(sd,(array+i)->addr,&tmp);
		printk("MN34227 0x%x value is 0x%x\n",(array+i)->addr,tmp);
	}
	return 0;
}
*/

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
static int sensor_write_array(struct v4l2_subdev *sd, struct regval_list *regs,
			      int array_size)
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
	unsigned char explow, expmid;

	struct sensor_info *info = to_state(sd);
	if (exp_val > 0x1fffff)
		exp_val = 0x1fffff;

	expmid = (unsigned char)((0x0ff000 & exp_val) >> 12);
	explow = (unsigned char)((0x000ff0 & exp_val) >> 4);

	sensor_write(sd, 0x0202, expmid);
	sensor_write(sd, 0x0203, explow);


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

unsigned short gain2db[497] = {
	0, 6, 11, 16, 21, 25, 30, 34, 38, 41, 45, 48, 52, 55, 58, 61, 64,
	67, 70, 73, 75, 78, 80, 83, 85, 87, 89, 92, 94, 96, 98, 100, 102, 104,
	    106, 107, 109, 111, 113,
	114, 116, 118, 119, 121, 122, 124, 125, 127, 128, 130, 131, 133, 134,
	    135, 137, 138, 139, 141,
	142, 143, 144, 146, 147, 148, 149, 150, 151, 153, 154, 155, 156, 157,
	    158, 159, 160, 161, 162,
	163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 173, 174, 175,
	    176, 177, 178, 179, 179,
	180, 181, 182, 183, 184, 184, 185, 186, 187, 187, 188, 189, 190, 190,
	    191, 192, 193, 193, 194,
	195, 196, 196, 197, 198, 198, 199, 200, 200, 201, 202, 202, 203, 204,
	    204, 205, 205, 206, 207,
	207, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215,
	    216, 216, 217, 217, 218,
	218, 219, 219, 220, 221, 221, 222, 222, 223, 223, 224, 224, 225, 225,
	    226, 226, 227, 227, 228,
	228, 229, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234, 234, 234,
	    235, 235, 236, 236, 237,
	237, 238, 238, 239, 239, 239, 240, 240, 241, 241, 242, 242, 242, 243,
	    243, 244, 244, 245, 245,
	245, 246, 246, 247, 247, 247, 248, 248, 249, 249, 249, 250, 250, 251,
	    251, 251, 252, 252, 252,
	253, 253, 254, 254, 254, 255, 255, 255, 256, 256, 257, 257, 257, 258,
	    258, 258, 259, 259, 259,
	260, 260, 260, 261, 261, 261, 262, 262, 262, 263, 263, 264, 264, 264,
	    265, 265, 265, 266, 266,
	266, 266, 267, 267, 267, 268, 268, 268, 269, 269, 269, 270, 270, 270,
	    271, 271, 271, 272, 272,
	272, 272, 273, 273, 273, 274, 274, 274, 275, 275, 275, 276, 276, 276,
	    276, 277, 277, 277, 278,
	278, 278, 278, 279, 279, 279, 280, 280, 280, 280, 281, 281, 281, 282,
	    282, 282, 282, 283, 283,
	283, 283, 284, 284, 284, 285, 285, 285, 285, 286, 286, 286, 286, 287,
	    287, 287, 287, 288, 288,
	288, 288, 289, 289, 289, 289, 290, 290, 290, 291, 291, 291, 291, 292,
	    292, 292, 292, 292, 293,
	293, 293, 293, 294, 294, 294, 294, 295, 295, 295, 295, 296, 296, 296,
	    296, 297, 297, 297, 297,
	298, 298, 298, 298, 298, 299, 299, 299, 299, 300, 300, 300, 300, 301,
	    301, 301, 301, 301, 302,
	302, 302, 302, 303, 303, 303, 303, 303, 304, 304, 304, 304, 304, 305,
	    305, 305, 305, 306, 306,
	306, 306, 306, 307, 307, 307, 307, 307, 308, 308, 308, 308, 309, 309,
	    309, 309, 309, 310, 310,
	310, 310, 310, 311, 311, 311, 311, 311, 312, 312, 312, 312, 312, 313,
	    313, 313, 313, 313, 314,
	314, 314, 314, 314, 315, 315, 315, 315, 315, 316, 316, 316, 316, 316,
	    316, 317, 317, 317, 317,
	317, 318, 318, 318, 318, 318, 319, 319, 319, 319, 319, 319, 320, 320,
	    320, 320, 320, 320, 320, 320, 320,
};
static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);

	if (gain_val < 1 * 16)
		gain_val = 16;
	if (gain_val > 32 * 16 - 1)
		gain_val = 32 * 16 - 1;

	sensor_write(sd, 0x0204, 1 + gain2db[gain_val - 16] / 256);
	sensor_write(sd, 0x0205, (gain2db[gain_val - 16]) & 0xff);

	info->gain = gain_val;

	return 0;
}

static int mn34227_sensor_vts;
static int sensor_s_exp_gain(struct v4l2_subdev *sd,
			     struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val, shutter, frame_length;
	struct sensor_info *info = to_state(sd);
	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if (gain_val < 1 * 16)
		gain_val = 16;
	if (gain_val > 64 * 16 - 1)
		gain_val = 64 * 16 - 1;
	if (exp_val > 0xfffff)
		exp_val = 0xfffff;
	shutter = exp_val / 16;
	if (shutter > mn34227_sensor_vts - 2)
		frame_length = shutter + 2;
	else
		frame_length = mn34227_sensor_vts;

	sensor_s_exp(sd, exp_val);
	sensor_s_gain(sd, gain_val);

	printk("mn34227 sensor_set_gain exp= %d, %d Done!\n", gain_val,
	       exp_val);

	info->exp = exp_val;
	info->gain = gain_val;
	return 0;
}

static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	unsigned char rdval;
	ret = sensor_read(sd, 0x3001, &rdval);
	if (ret != 0)
		return ret;
	if (on_off == CSI_STBY_ON)
		ret = sensor_write(sd, 0x3001, rdval & 0xfe);
	else
		ret = sensor_write(sd, 0x3001, rdval | 0x01);
	return ret;
}

/*
 * Stuff that knows about the sensor.
 */
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;

	/*
	 * insure that clk_disable() and clk_enable() are called in pair
	 * when calling CSI_SUBDEV_STBY_ON/OFF and CSI_SUBDEV_PWR_ON/OFF
	 */
	ret = 0;
	switch (on) {
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
		ret = sensor_s_sw_stby(sd, CSI_STBY_ON);
		if (ret < 0)
			vfe_dev_err("soft stby falied!\n");
		usleep_range(10000, 12000);
		cci_lock(sd);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		cci_unlock(sd);
		vfe_set_mclk(sd, OFF);
		break;
	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
		cci_lock(sd);
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		ret = sensor_s_sw_stby(sd, CSI_STBY_OFF);
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

		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);

		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
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
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(10000, 12000);
		break;
	case 1:
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	unsigned char rdval = 0;
	LOG_ERR_RET(sensor_read(sd, 0x0008, &rdval))
	return 0;

}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);
	vfe_dev_dbg("sensor_init\n");

	/*Make sure it is a target sensor */
	ret = sensor_detect(sd);
	if (ret) {
		vfe_dev_err("chip found is not an target chip.\n");
		return ret;
	}

	vfe_get_standby_mode(sd, &info->stby_mode);
	if ((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY)
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
	info->tpf.denominator = 30;	/* 30fps */

	ret =
	    sensor_write_array(sd, sensor_default_regs,
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
			memcpy(arg,
			       info->current_wins,
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
	int bpp;		/* Bytes per pixel */
} sensor_formats[] = {
	{
			.desc = "Raw RGB Bayer",
			.mbus_code = V4L2_MBUS_FMT_SGRBG12_12X1,
			.regs = sensor_fmt_raw,
			.regs_size = ARRAY_SIZE(sensor_fmt_raw),
			.bpp = 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */

static struct sensor_win_size sensor_win_sizes[] = {
	/* 1080P */

	{
	 .width = HD1080_WIDTH,
	 .height = HD1080_HEIGHT,
	 .hoffset = 0,
	 .voffset = 0,
	 .hts = 2200,
	 .vts = VMAX,
	 .pclk = 74250000,
	 .mipi_bps = 222 * 1000 * 1000,
	 .fps_fixed = 30,
	 .bin_factor = 1,
	 .intg_min = 1 << 4,
	 .intg_max = (VMAX - 2) << 4,
	 .gain_min = 1 << 4,
	 .gain_max = 32 << 4,
	 .regs = sensor_1080p30_regs,
	 .regs_size = ARRAY_SIZE(sensor_1080p30_regs),
	 .set_size = NULL,
	 },

	/* 720p */
	{
	 .width = HD1080_WIDTH,
	 .height = HD1080_HEIGHT,
	 .hoffset = 0,
	 .voffset = 0,
	 .hts = 2200,
	 .vts = VMAX,
	 .pclk = 74250000,
	 .mipi_bps = 222 * 1000 * 1000,
	 .fps_fixed = 30,
	 .bin_factor = 1,
	 .intg_min = 1 << 4,
	 .intg_max = (VMAX - 2) << 4,
	 .gain_min = 1 << 4,
	 .gain_max = 32 << 4,
	 .width_input = 1920,
	 .height_input = 1080,
	 .regs = sensor_1080p30_regs,
	 .regs_size = ARRAY_SIZE(sensor_1080p30_regs),
	 .set_size = NULL,
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
	if (fsize->index > N_WIN_SIZES - 1)
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = sensor_win_sizes[fsize->index].width;
	fsize->discrete.height = sensor_win_sizes[fsize->index].height;

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
		if (fmt->width >= wsize->width && fmt->height >= wsize->height)
			break;

	if (wsize >= sensor_win_sizes + N_WIN_SIZES)
		wsize--;	/* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;
	/*
	 * Note the size we'll actually handle.
	 */
	fmt->width = wsize->width;
	fmt->height = wsize->height;
	info->current_wins = wsize;
	/* pix->bytesperline = pix->width*sensor_formats[index].bpp; */
	/* pix->sizeimage = pix->height*pix->bytesperline; */

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
	cfg->flags = 0 | V4L2_MBUS_CSI2_4_LANE | V4L2_MBUS_CSI2_CHANNEL_0;

	return 0;
}

/*
 * Set a format.
 */
static int sensor_s_fmt(struct v4l2_subdev *sd, struct v4l2_mbus_framefmt *fmt)
{
	int ret;
	struct sensor_format_struct *sensor_fmt;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_s_fmt\n");

	ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
	if (ret)
		return ret;

	/*
	if (info->capture_mode == V4L2_MODE_VIDEO) {
		//video
	} else if (info->capture_mode == V4L2_MODE_IMAGE) {
		//image
	}
	*/

	LOG_ERR_RET(sensor_write_array
		    (sd, sensor_fmt->regs, sensor_fmt->regs_size))

	    ret = 0;
	if (wsize->regs) {
		LOG_ERR_RET(sensor_write_array
			    (sd, wsize->regs, wsize->regs_size))
	}
	if (wsize->set_size)
		LOG_ERR_RET(wsize->set_size(sd))

		    info->fmt = sensor_fmt;
	info->width = wsize->width;
	info->height = wsize->height;
	mn34227_sensor_vts = wsize->vts;

	vfe_dev_print("s_fmt set width = %d, height = %d\n", wsize->width,
		      wsize->height);

	/*
	if (info->capture_mode == V4L2_MODE_VIDEO) {
		//video
	} else {
		//capture image
	}
	*/

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
	return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);
	vfe_dev_dbg("sensor_s_parm\n");
	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	if (info->tpf.numerator == 0)
		return -EINVAL;
	info->capture_mode = cp->capturemode;
	return 0;
}

static int sensor_queryctrl(struct v4l2_subdev *sd, struct v4l2_queryctrl *qc)
{
	/* Fill in min, max, step and default value for these controls. */
	/* see include/linux/videodev2.h for details */

	switch (qc->id) {
	case V4L2_CID_GAIN:
		return v4l2_ctrl_query_fill(qc, 1 * 16, 128 * 16 - 1, 1, 16);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 1, 65536 * 16, 1, 1);
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
	{SENSOR_NAME, 0},
	{}
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
