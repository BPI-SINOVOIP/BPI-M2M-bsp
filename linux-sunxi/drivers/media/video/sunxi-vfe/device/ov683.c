/*
 * A V4L2 driver for OV683 cameras.
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
MODULE_DESCRIPTION("A low-level driver for OV683 sensors");
MODULE_LICENSE("GPL");

/*for internel driver debug*/
#define DEV_DBG_EN      0
#if (DEV_DBG_EN == 1)
#define vfe_dev_dbg(x, arg...)	printk(KERN_DEBUG"[OV683]"x, ##arg)
#else
#define vfe_dev_dbg(x, arg...)
#endif
#define vfe_dev_err(x, arg...)	printk(KERN_ERR"[OV683]"x, ##arg)
#define vfe_dev_print(x, arg...) printk(KERN_INFO"[OV683]"x, ##arg)

#define LOG_ERR_RET(x)  { \
		int ret;  \
		ret = x; \
		if (ret < 0) {\
			vfe_dev_err("error at %s\n", __func__);  \
			return ret; \
		} \
	}

/*define module timing*/

#define MCLK              (24*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_LOW
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR  0x683

/*define the voltage level of control signal*/
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

/*
 * Our nominal (default) frame rate.
 */
#define SENSOR_FRAME_RATE 30
#define OV683_WRITE_ADDR (0x84)
#define OV683_READ_ADDR  (0x85)

static struct v4l2_subdev *glb_sd;
#define SENSOR_NAME "ov683"

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

static struct regval_list sensor_4M_regs[] = {
#ifdef CLOLOR_BAR
	{0x2080, 0x80},
	{0x3080, 0x80},
#endif
};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */
static struct regval_list sensor_fmt_mipi_yuv422[] = {
};

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

static int reg_val_show(struct v4l2_subdev *sd, unsigned short reg)
{
	unsigned char tmp;
	sensor_read(sd, reg, &tmp);
	printk("[richard debug]0x%x value is 0x%x\n", reg, tmp);
	return tmp;
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

static int reg_val_store(struct v4l2_subdev *sd, unsigned short reg,
			 unsigned char value)
{
	vfe_dev_dbg("write 0x%x value is 0x%x\n", reg, value);
	sensor_write(sd, reg, value);

	return 0;
}

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

/****************************************************
  * 0x00 cmd is not ready -wait,
  * 0x88-cmd finished and successes
  *0x55-cmd finished and failed
  *
  *****************************************************
  */
void stream_control(struct v4l2_subdev *sd, int on)
{
	if (on != 0) {
		vfe_dev_dbg("stream_control on :\n");
		reg_val_store(sd, 0x1500, 0x05);  /*cmd id*/
		reg_val_store(sd, 0x1510, 0x01);  /*on*/
		reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
		reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/

		reg_val_store(sd, 0x0230, 0x00);
		reg_val_store(sd, 0x0231, 0x00);
		reg_val_store(sd, 0x0232, 0x00);
		reg_val_store(sd, 0x0233, 0x00);
	} else {
		vfe_dev_dbg("stream_control off :\n");
		reg_val_store(sd, 0x1500, 0x05);
		reg_val_store(sd, 0x1510, 0x00);
		reg_val_store(sd, 0x150e, 0x00);
	}
}

/****************************************************
 * 0x00 cmd is not ready-wait,
 * 0xff-cmd finished and successes
 *0x55-cmd finished and failed
 *
 *****************************************************
 */

void read_ov683_reg_access(struct v4l2_subdev *sd)
{
#ifdef DEBUG_OV683
	unsigned char tmp = 0;
	int i = 0;
	vfe_dev_dbg("A,Read sensor register :\n");
	reg_val_store(sd, 0x1500, 0xE4);  /*cmd id*/
	reg_val_store(sd, 0x1510, 0x20);  /*sensor id*/
	reg_val_store(sd, 0x1511, 0x30);  /*sensor addr_h*/
	reg_val_store(sd, 0x1512, 0x0A);  /*sensor addr_l*/
	reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
	reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/

	while ((reg_val_show(sd, 0x150e) != 0xff) && (i++ < 10))
		msleep(20);

	reg_val_show(sd, 0x1514);
	reg_val_store(sd, 0x1500, 0xE4);  /*cmd id*/
	reg_val_store(sd, 0x1510, 0x20);  /*sensor id*/
	reg_val_store(sd, 0x1511, 0x38);  /*sensor addr_h*/
	reg_val_store(sd, 0x1512, 0x0e);  /*sensor addr_l*/
	reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
	reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/

	while ((reg_val_show(sd, 0x150e) != 0xff) && (i++ < 10))
		msleep(20);

	reg_val_show(sd, 0x1514);
	reg_val_store(sd, 0x1500, 0xE4);  /*cmd id*/
	reg_val_store(sd, 0x1510, 0x20);  /*sensor id*/
	reg_val_store(sd, 0x1511, 0x38);  /*sensor addr_h*/
	reg_val_store(sd, 0x1512, 0x0f);  /*sensor addr_l*/
	reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
	reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/

	while ((reg_val_show(sd, 0x150e) != 0xff) && (i++ < 10))
		msleep(20);

	reg_val_show(sd, 0x1514);

#endif
}

/****************************************************
 * 0x00 cmd is not ready-wait,
 * 0xff-cmd finished and successes
 *0x55-cmd finished and failed
 *
 *****************************************************
 */
void write_ov683_reg(struct v4l2_subdev *sd)
{
#ifdef DEBUG_OV683
	reg_val_store(sd, 0x1500, 0xE5);  /*cmd id*/
	reg_val_store(sd, 0x1510, 0x20);  /*sensor id*/
	reg_val_store(sd, 0x1511, 0x38);  /*sensor addr_h*/
	reg_val_store(sd, 0x1512, 0x0e);  /*sensor addr_l*/
	reg_val_store(sd, 0x1513, 0x0c);  /*sensor data*/
	reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
	reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/
	reg_val_show(sd, 0x150e);         /*read back*/
	msleep(20);
	reg_val_show(sd, 0x150e);

	reg_val_store(sd, 0x1500, 0xE5);  /*cmd id*/
	reg_val_store(sd, 0x1510, 0x20);  /*sensor id*/
	reg_val_store(sd, 0x1511, 0x38);  /*sensor addr_h*/
	reg_val_store(sd, 0x1512, 0x0f);  /*sensor addr_l*/
	reg_val_store(sd, 0x1513, 0x24);  /*sensor data*/
	reg_val_store(sd, 0x150e, 0x00);  /*clean flag*/
	reg_val_store(sd, 0x150f, 0x80);  /*issue cmd*/
	reg_val_show(sd, 0x150e);         /*read back*/
	msleep(20);
	reg_val_show(sd, 0x150e);
#endif
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
	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->gain;

	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	vfe_dev_dbg("==== window size\n");
	reg_val_show(sd, 0x76ba);  /*0x05   window size*/
	reg_val_show(sd, 0x76bb);  /*0xf0*/
	reg_val_show(sd, 0x76c2);  /*0x0a    window size*/
	reg_val_show(sd, 0x76c3);  /*0x80*/

	return 0;
}

static int ov683_sensor_vts;
static int sensor_s_exp_gain(struct v4l2_subdev *sd,
			     struct sensor_exp_gain *exp_gain)
{
	return 0;
}

static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret = 0;
	return ret;
}

static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;
	ret = 0;

	switch (on) {
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
		/*software standby on*/
		ret = sensor_s_sw_stby(sd, CSI_GPIO_HIGH);
		if (ret < 0)
			vfe_dev_err("soft stby falied!\n");
		usleep_range(10000, 12000);
		/*
		*when using i2c_lock_adpater function,
		*the following codes must not access i2c bus
		*before calling i2c_unlock_adapter
		*/
		cci_lock(sd);
		/*standby on io*/
		vfe_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		/*
		*remember to unlock i2c adapter,
		*so the device can access the i2c bus again
		*/
		cci_unlock(sd);
		/*inactive mclk after stadby in*/
		vfe_set_mclk(sd, OFF);
		break;
	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
		cci_lock(sd);
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		usleep_range(10000, 12000);
		ret = sensor_s_sw_stby(sd, CSI_GPIO_LOW);
		if (ret < 0)
			vfe_dev_err("soft stby off falied!\n");
		cci_unlock(sd);
		break;
	case CSI_SUBDEV_PWR_ON:
		vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
		cci_lock(sd);
		/*power on reset*/
		vfe_gpio_set_status(sd, PWDN, 1);/*set the gpio to output*/
		vfe_gpio_set_status(sd, RESET, 1);/*set the gpio to output*/
		vfe_gpio_set_status(sd, POWER_EN, 1);/*set the gpio to output*/

		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		usleep_range(1000, 1200);
		/*power supply*/
		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_HIGH);
		vfe_set_pmu_channel(sd, AVDD, ON);
		usleep_range(7000, 8000);
		/*reset on io*/
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(1000, 1200);
		/*active mclk before power on*/
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		cci_unlock(sd);
		break;
	case CSI_SUBDEV_PWR_OFF:
		vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");
		vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");

		stream_control(sd, 0);

		cci_lock(sd);
		vfe_gpio_set_status(sd, PWDN, 1);/*set the gpio to output*/
		vfe_gpio_set_status(sd, RESET, 1);/*set the gpio to output*/
		vfe_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		/*inactive mclk before power off*/
		vfe_set_mclk(sd, OFF);
		/*power supply off*/
		vfe_set_pmu_channel(sd, AFVDD, OFF);
		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		/*set the io to hi-z*/
		vfe_gpio_set_status(sd, RESET, 0);/*set the gpio to input*/
		vfe_gpio_set_status(sd, PWDN, 0);/*set the gpio to input*/
		vfe_gpio_set_status(sd, POWER_EN, 0);/*set the gpio to input*/
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
	unsigned char rdval = 0;

	vfe_dev_dbg("sensor_detect_1--!\n");
	LOG_ERR_RET(sensor_read(sd, 0x2080, &rdval));
	vfe_dev_dbg("sensor_detect_2--! 0x2080 = %x\n", rdval);
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

	info->width = 2688;
	info->height = 1520;

	info->hflip = 0;
	info->vflip = 0;
	info->gain = 0;

	info->tpf.numerator = 1;
	info->tpf.denominator = 30;

	if (info->stby_mode == 0)
		info->init_first_flag = 0;

	info->preview_first_flag = 1;

#ifdef CLOLOR_BAR
	reg_val_store(sd, 0x3080, 0x80);
	reg_val_store(sd, 0x2080, 0x80);
#endif
	reg_val_show(sd, 0x0230);
	reg_val_show(sd, 0x0231);
	reg_val_show(sd, 0x0232);
	reg_val_show(sd, 0x0233);
	reg_val_show(sd, 0x2080);
	reg_val_show(sd, 0x3080);

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
	int bpp; /* Bytes per pixel */
} sensor_formats[] = {
	{
		.desc = "MIPI YUV422",
		.mbus_code = V4L2_MBUS_FMT_YUYV8_16X1,
		.regs = sensor_fmt_mipi_yuv422,
		.regs_size = ARRAY_SIZE(sensor_fmt_mipi_yuv422),
		.bpp = 2,
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.
 * Try to capture the info here.
 */

static struct sensor_win_size sensor_win_sizes[] = {
	/* qsxga: 2688*1520 */
	{
	 .width = 2688,
	 .height = 1520,

	 .regs = sensor_4M_regs,
	 .hoffset = 0,
	 .voffset = 0,
	 .mipi_bps = 720 * 1000 * 1000,
	 .regs_size = ARRAY_SIZE(sensor_4M_regs),
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

static int sensor_s_fmt(struct v4l2_subdev *sd, struct v4l2_mbus_framefmt *fmt)
{
	int ret = 0;
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
	info->exp = 0;
	info->gain = 0;
	ov683_sensor_vts = wsize->vts;

	vfe_dev_print("s_fmt set width = %d, height = %d\n", wsize->width,
		      wsize->height);

	/*
	if (info->capture_mode == V4L2_MODE_VIDEO) {
		//video
	} else {
		//capture image
	}
	*/

	stream_control(sd, 1);

#ifdef CLOLOR_BAR
	msleep(20);
	reg_val_store(sd, 0x3080, 0x80);
	reg_val_store(sd, 0x2080, 0x80);
#endif


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
		return v4l2_ctrl_query_fill(qc, 1 * 16, 16 * 16, 1, 16);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 1, 65536 * 16, 1, 1);
	case V4L2_CID_FRAME_RATE:
		return v4l2_ctrl_query_fill(qc, 15, 120, 1, 30);
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

	/*
	if (ctrl->value < qc.minimum || ctrl->value > qc.maximum)
		return -ERANGE;
	*/

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->value);

	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->value);

	/*
	case V4L2_CID_FRAME_RATE:
		return sensor_s_framerate(sd, ctrl->value);
	*/
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
