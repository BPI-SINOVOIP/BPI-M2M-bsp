/*
 * codec tas5707 driver
 *
 * Copyright(c) 2015-2018 Allwinnertech Co., Ltd.
 *      http://www.allwinnertech.com
 *
 * Author: huangxin <huangxin@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/firmware.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>
#include <sound/jack.h>
#include <mach/sys_config.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <linux/io.h>
#include <linux/regulator/consumer.h>
#include "tas5707.h"

#define I2C_CHANNEL_NUM 0
#define AUX_SINGLE_TIME 500
/*
* 1: 500ms
* 2: 1000ms
* 3: 1500ms
* 4: 2000ms
* ......
*/
#define AUX_SIGNAL_DEBOUNCRE_COUNT	2

/*for debounce detect*/
static int g_aux_nosignal_count;
static int g_aux_signal_count;

static bool AUX_JACK_DETECT;
static bool AUX_SIGNAL_DETECT;

static bool tas5707_linein_en;
static bool tas5707_playing_en;
static bool aux_signal;
static int tas5707_used;
/*default vol step is 10*/
static int vol_step				= 10;
static int master_vol;

/*for pa gpio ctrl*/
static script_item_u pa_item;
static script_item_u reset_item;
static script_item_u g_aux_signal_item;
static script_item_u chip_en;
static script_item_u tas5707_power0;
static script_item_u tas5707_power1;

struct timer_list g_aux_signal_timer;
static struct i2c_client *tas5707_client;
static struct snd_soc_codec *g_codec;

struct tas5707_priv {
	int jack_status;
	struct i2c_client *i2c;
	struct snd_soc_codec *codec;
	struct work_struct linein_jack;
	struct snd_soc_jack jack;
};

struct _tas5707_vol_table {
	int vtable_step;
	float volgain_db;
	int vol_reg;
};

/*
* low vol step: 0~13 step bq1
*/
unsigned char eq_lvolstep_bq1_29[] = {
	0x00, 0x7F, 0x69, 0xE0,
	0x03, 0x01, 0x2C, 0x40,
	0x00, 0x7F, 0x69, 0xE0,
	0x00, 0xFE, 0xD2, 0x61,
	0x03, 0x81, 0x2A, 0xE2,
};

unsigned char eq_lvolstep_bq1_2a[] = {
	0x00, 0x80, 0x46, 0x69,
	0x03, 0x01, 0x58, 0xF0,
	0x00, 0x7E, 0x64, 0x39,
	0x00, 0xFE, 0xA7, 0x10,
	0x03, 0x81, 0x55, 0x5D,
};

unsigned char eq_lvolstep_bq1_2b[] = {
	0x00, 0x7F, 0xCD, 0xF3,
	0x03, 0x02, 0x7F, 0xCD,
	0x00, 0x7D, 0xBA, 0xF0,
	0x00, 0xFD, 0x80, 0x33,
	0x03, 0x82, 0x77, 0x1D,
};

unsigned char eq_lvolstep_bq1_2c[] = {
	0x00, 0x80, 0x72, 0x23,
	0x03, 0x03, 0x94, 0x43,
	0x00, 0x7C, 0x1C, 0x37,
	0x00, 0xFC, 0x6B, 0xBD,
	0x03, 0x83, 0x71, 0xA5,
};

unsigned char eq_lvolstep_bq1_2d[] = {
	0x00, 0x85, 0x28, 0x41,
	0x03, 0x0E, 0x6B, 0xFE,
	0x00, 0x6D, 0x96, 0xFD,
	0x00, 0xF1, 0x94, 0x02,
	0x03, 0x8D, 0x40, 0xC1,
};

unsigned char eq_lvolstep_bq1_2e[] = {
	0x00, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

unsigned char eq_lvolstep_bq1_2f[] = {
	0x00, 0x8C, 0x21, 0x87,
	0x03, 0xAE, 0xD9, 0xEC,
	0x00, 0x16, 0x2A, 0xA0,
	0x00, 0x51, 0x26, 0x14,
	0x03, 0xDD, 0xB3, 0xD8,
};

/*
* low vol step: 0~13 step bq2
*/
unsigned char eq_lvolstep_bq2_30[] = {
	0x00, 0x7F, 0x69, 0xE0,
	0x03, 0x01, 0x2C, 0x40,
	0x00, 0x7F, 0x69, 0xE0,
	0x00, 0xFE, 0xD2, 0x61,
	0x03, 0x81, 0x2A, 0xE2,
};

unsigned char eq_lvolstep_bq2_31[] = {
	0x00, 0x80, 0x46, 0x69,
	0x03, 0x01, 0x58, 0xF0,
	0x00, 0x7E, 0x64, 0x39,
	0x00, 0xFE, 0xA7, 0x10,
	0x03, 0x81, 0x55, 0x5D,
};

unsigned char eq_lvolstep_bq2_32[] = {
	0x00, 0x7F, 0xCD, 0xF3,
	0x03, 0x02, 0x7F, 0xCD,
	0x00, 0x7D, 0xBA, 0xF0,
	0x00, 0xFD, 0x80, 0x33,
	0x03, 0x82, 0x77, 0x1D,
};

unsigned char eq_lvolstep_bq2_33[] = {
	0x00, 0x80, 0x72, 0x23,
	0x03, 0x03, 0x94, 0x43,
	0x00, 0x7C, 0x1C, 0x37,
	0x00, 0xFC, 0x6B, 0xBD,
	0x03, 0x83, 0x71, 0xA5,
};

unsigned char eq_lvolstep_bq2_34[] = {
	0x00, 0x85, 0x28, 0x41,
	0x03, 0x0E, 0x6B, 0xFE,
	0x00, 0x6D, 0x96, 0xFD,
	0x00, 0xF1, 0x94, 0x02,
	0x03, 0x8D, 0x40, 0xC1,
};

unsigned char eq_lvolstep_bq2_35[] = {
	0x00, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

unsigned char eq_lvolstep_bq2_36[] = {
	0x00, 0x8C, 0x21, 0x87,
	0x03, 0xAE, 0xD9, 0xEC,
	0x00, 0x16, 0x2A, 0xA0,
	0x00, 0x51, 0x26, 0x14,
	0x03, 0xDD, 0xB3, 0xD8,
};

/*
* tas5707 DRC
* write 0x00, 0x00, 0x00, 0x01 can enable DRC fucntion
*/
unsigned char drc_ctl46[] = {
	0x00, 0x00, 0x00, 0x01,
};

unsigned char drc_cfg42[] = {
	0x00, 0x0A, 0x65, 0x73,
};

unsigned char drc_cfg41[] = {
	0x03, 0x82, 0x8F, 0x5D,
};

unsigned char drc_cfg40[] = {
	0xFD, 0x8C, 0xD1, 0xEA,
};

unsigned char drc_cfg3c[] = {
	0x00, 0x00, 0x06, 0xD3,
	0x00, 0x7F, 0xF9, 0x2B,
};

unsigned char drc_cfg3b[] = {
	0x00, 0x02, 0xA3, 0x99,
	0x00, 0x7D, 0x5C, 0x65,
};

unsigned char drc_cfg3a[] = {
	0x00, 0x7F, 0xFF, 0x03,
	0x00, 0x00, 0x00, 0xFB,
};

unsigned char bdmode_cfg20[] = {
	0x00, 0x89, 0x77, 0x72,
};

/*
* TAS5707 volume table
* step size is 0.5dB
*/
static const struct _tas5707_vol_table tas5707_vol_table[] = {
	{16, 0, 0x30},
	{15, -1, 0x32},
	{14, -2, 0x34},
	{13, -3.5, 0x37},
	{12, -5, 0x3a},
	{11, -7, 0x3e},
	{10, -10, 0x44},
	{9, -13, 0x4a},
	{8, -17, 0x52},
	{7, -21, 0x5a},
	{6, -26, 0x64},
	{5, -31, 0x6e},
	{4, -36, 0x78},
	{3, -42, 0x84},
	{2, -50, 0x94},
	{1, -60, 0xa8},
	{0, -103, 0xff},
};

static int set_eq_config(void)
{
	int ret = 0;

	/*
	* low vol step: 0~13 step bq1
	*/
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x29, 20,\
			eq_lvolstep_bq1_29);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2a, 20,\
			eq_lvolstep_bq1_2a);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2b, 20,\
			eq_lvolstep_bq1_2b);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2c, 20,\
			eq_lvolstep_bq1_2c);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2d, 20,\
			eq_lvolstep_bq1_2d);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2e, 20,\
			eq_lvolstep_bq1_2e);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x2f, 20,\
			eq_lvolstep_bq1_2f);
	if (ret < 0)
		return ret;

	/*
	* low vol step: 0~13 step bq2
	*/
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x30, 20,\
			eq_lvolstep_bq2_30);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x31, 20,\
			eq_lvolstep_bq2_31);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x32, 20,\
			eq_lvolstep_bq2_32);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x33, 20,\
			eq_lvolstep_bq2_33);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x34, 20,\
			eq_lvolstep_bq2_34);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x35, 20,\
			eq_lvolstep_bq2_35);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x36, 20,\
			eq_lvolstep_bq2_36);
	if (ret < 0)
		return ret;

	return 0;
}

static int tas5707_set_vol(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	int ret = 0;
	int i = 0;

	vol_step = ucontrol->value.integer.value[0];
	/*
	* pa PDN gpio control must enable for tas5707 reg read write.
	*/
	gpio_set_value(pa_item.gpio.gpio, 1);
	for (i = 0; ARRAY_SIZE(tas5707_vol_table); i++) {
		if (tas5707_vol_table[i].vtable_step == vol_step) {
			snd_soc_write(g_codec,\
				0x07, tas5707_vol_table[i].vol_reg);
			break;
		}
	}

	if ((tas5707_linein_en == 0) && (tas5707_playing_en == 0))
		gpio_set_value(pa_item.gpio.gpio, 0);

	return ret;
}

static int tas5707_get_vol(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = vol_step;
	return 0;
}

static const char *tas5707_vol_function[]
	= {"vol0", "vol1", "vol2", "vol3",\
		"vol4", "vol5", "vol6", "vol7",\
		"vol8", "vol9", "vol10", "vol11",\
		"vol12", "vol13", "vol14", "vol15", "vol16"};
static const struct soc_enum tas5707_vol_enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(tas5707_vol_function),\
		tas5707_vol_function),
};

/*
*	max:0x0:24db
*	default:0db:0x30
*	mute:0xff
*/
static const struct snd_kcontrol_new sunxi_codec_controls[] = {
	SOC_ENUM_EXT("Master Playback Volume", tas5707_vol_enum[0],\
		tas5707_get_vol, tas5707_set_vol),
};

static int drc_init(void)
{
	int ret = 0;

	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x46, 4,  drc_ctl46);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x42, 4,  drc_cfg42);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x41, 4,  drc_cfg41);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x40, 4,  drc_cfg40);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x3c, 8,  drc_cfg3c);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x3b, 8,  drc_cfg3b);
	if (ret < 0)
		return ret;
	ret = i2c_smbus_write_i2c_block_data(tas5707_client,\
		0x3a, 8,  drc_cfg3a);
	if (ret < 0)
		return ret;

	return 0;
}

static int bd_mode_init(void)
{
	int ret = 0;

	ret = i2c_smbus_write_i2c_block_data(tas5707_client, 0x20, 4,\
			bdmode_cfg20);
	if (ret < 0)
		return ret;

	return 0;
}

static int tas5707_startup(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	gpio_set_value(pa_item.gpio.gpio, 1);
	tas5707_playing_en = true;
	return 0;
}

static void tas5707_shutdown(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	if (tas5707_linein_en != true)
		gpio_set_value(pa_item.gpio.gpio, 0);

	tas5707_playing_en = false;
}

static int tas5707_hw_params(struct snd_pcm_substream *substream,
			    struct snd_pcm_hw_params *params,
			    struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;

	snd_soc_write(codec, 0x1b, 0x00);
	msleep(50);
	snd_soc_write(codec, 0x05, 0x00);

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		 /*I2S data format, 16-bit IIS*/
		snd_soc_write(codec, 0x04, 0x03);
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		 /*I2S data format, default 24-bit IIS*/
		snd_soc_write(codec, 0x04, 0x05);
		break;
	default:
		dev_err(dai->dev, "unsupport: %u\n", params_format(params));
		return -EINVAL;
	}

	return 0;
}

static int tas5707_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	return 0;
}

static int tas5707_set_sysclk(struct snd_soc_dai *dai,
			     int clk_id, unsigned int freq, int dir)
{
	return 0;
}

static int tas5707_set_clkdiv(struct snd_soc_dai *dai,
			     int div_id, int div)
{
	return 0;
}

/*
* Identify the jack type as Headset/Headphone/None
*/
static void sunxi_report_jack_type(struct snd_soc_jack *jack, int jack_status)
{
	struct tas5707_priv *tas5707 = container_of(jack,\
					struct tas5707_priv, jack);
	snd_jack_report(tas5707->jack.jack, jack_status);
}

static void aux_signal_events(unsigned long arg)
{
	int ret = 0;
	struct tas5707_priv *tas5707 = snd_soc_codec_get_drvdata(g_codec);

	/*
	* ret == 0, means has aux signal;
	* ret == 1, means hasn't aux signal;
	*/
	ret = gpio_get_value(g_aux_signal_item.gpio.gpio);
	if (!ret) {
		g_aux_signal_count++;
		/*aux signal*/
		if ((aux_signal == false) &&\
			(g_aux_signal_count >= AUX_SIGNAL_DEBOUNCRE_COUNT)) {
			g_aux_nosignal_count	= 0;
			aux_signal			= true;
			AUX_SIGNAL_DETECT	= true;
			tas5707->jack_status = SND_JACK_LINEIN_SIGNAL;
			sunxi_report_jack_type(&tas5707->jack,\
				tas5707->jack_status);
		}
	} else {
		/*
		*	no aux signal, g_aux_nosignal_count++;
		*/
		g_aux_nosignal_count++;
		/*aux signal -> no aux signal*/
		if ((aux_signal == true) &&\
			(g_aux_nosignal_count >= AUX_SIGNAL_DEBOUNCRE_COUNT)) {
			g_aux_signal_count = 0;
			aux_signal = false;
			tas5707->jack_status	= SND_JACK_LINEIN;
			AUX_SIGNAL_DETECT		= false;
			sunxi_report_jack_type(&tas5707->jack,\
				tas5707->jack_status);
		}
	}

	mod_timer(&g_aux_signal_timer,\
		jiffies + msecs_to_jiffies(AUX_SINGLE_TIME));
}

static void tas5707_linein_work(struct work_struct *work)
{
	struct tas5707_priv *tas5707 = container_of(work,\
			struct tas5707_priv, linein_jack);
	struct snd_soc_codec *codec = tas5707->codec;

	snd_soc_write(codec, 0x1b, 0x00); /*auto-detect fs*/
	msleep(50);
	snd_soc_write(codec, 0x05, 0x00); /*exit shutdown, normal operation*/
	snd_soc_write(codec, 0x04, 0x03); /*I2S data format, 16-bit IIS*/
}

/*
1.plugin linein, enable PA,audio route is:linein->SPK
2.plugout linein, disable PA,
	audio route is local audio(include wifi audio and bt audio) -> SPK;
*/
void tas5707_linein_play(bool on)
{
	struct tas5707_priv *tas5707 = NULL;
	if (tas5707_used) {
		tas5707 = snd_soc_codec_get_drvdata(g_codec);

		if (on) {
			tas5707_linein_en = true;
			gpio_set_value(pa_item.gpio.gpio, 1);

			AUX_JACK_DETECT = true;
			setup_timer(&g_aux_signal_timer,\
				aux_signal_events, (unsigned long)0);
			mod_timer(&g_aux_signal_timer,\
				jiffies + msecs_to_jiffies(AUX_SINGLE_TIME));
			tas5707->jack_status = SND_JACK_LINEIN;
			/*
			* pa PDN gpio control
			* must enable for tas5707 reg read/write.
			* so tas5707_linein_work must run after
			* gpio_set_value(pa_item.gpio.gpio, 1);
			*/
			schedule_work(&tas5707->linein_jack);
		} else {
			tas5707_linein_en = false;
			del_timer(&g_aux_signal_timer);
			if (tas5707_playing_en != true)
				gpio_set_value(pa_item.gpio.gpio, 0);

			AUX_JACK_DETECT		= false;
			AUX_SIGNAL_DETECT	= false;
			g_aux_nosignal_count = 0;
			g_aux_signal_count	= 0;
			aux_signal			= false;
			tas5707->jack_status = SND_JACK_LINEIN_PLUGOUT;
		}

		sunxi_report_jack_type(&tas5707->jack, tas5707->jack_status);
	}
}
EXPORT_SYMBOL(tas5707_linein_play);

static const struct snd_soc_dai_ops tas5707_dai_ops = {
	.hw_params	= tas5707_hw_params,
	.set_fmt	= tas5707_set_fmt,
	.set_sysclk = tas5707_set_sysclk,
	.set_clkdiv = tas5707_set_clkdiv,
	.startup	= tas5707_startup,
	.shutdown	= tas5707_shutdown,
};

#define tas5707_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
			SNDRV_PCM_FMTBIT_S24_LE)
#define tas5707_RATES SNDRV_PCM_RATE_8000_96000

static struct snd_soc_dai_driver tas5707_dai0 = {
		.name = "tas5707-pcm0",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = tas5707_RATES,
			.formats = tas5707_FORMATS,
		},
		.ops = &tas5707_dai_ops,
};

#ifdef CONFIG_PM
static int tas5707_suspend(struct snd_soc_codec *codec)
{
	/*
	* pa PDN gpio control must enable for tas5707 reg read write.
	*/
	gpio_set_value(pa_item.gpio.gpio, 1);
	master_vol = snd_soc_read(codec, 0x07);
	gpio_set_value(pa_item.gpio.gpio, 0);
	return 0;
}

static int tas5707_resume(struct snd_soc_codec *codec)
{
	int ret = 0;
	/*
	* pa PDN gpio control must enable for tas5707 reg read write.
	*/
	gpio_set_value(pa_item.gpio.gpio, 1);
	msleep(20);
	/*auto-detect fs*/
	snd_soc_write(codec, 0x1b, 0x00);
	msleep(50);

	ret = drc_init();
	if (ret < 0)
		return ret;


	ret = bd_mode_init();
	if (ret < 0)
		return ret;

	snd_soc_write(codec, 0x11, 0xb8);
	snd_soc_write(codec, 0x12, 0x60);
	snd_soc_write(codec, 0x13, 0xa0);
	snd_soc_write(codec, 0x14, 0x48);
	snd_soc_write(codec, 0x07, master_vol);
	msleep(20);
	/* EQ */
	ret = set_eq_config();
	if (ret < 0)
		return ret;

	gpio_set_value(pa_item.gpio.gpio, 0);

	return 0;
}
#else
#define tas5707_suspend NULL
#define tas5707_resume NULL
#endif

static int tas5707_probe(struct snd_soc_codec *codec)
{
	struct tas5707_priv *tas5707 = dev_get_drvdata(codec->dev);
	s32 ret = 0;
	int i = 0;

	ret = snd_soc_codec_set_cache_io(codec, 8, 8, CONFIG_REGMAP_I2C);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set cache I/O: %d\n", ret);
		return ret;
	}
	tas5707->codec = codec;
	g_codec = codec;
	snd_soc_add_codec_controls(codec, sunxi_codec_controls,
					ARRAY_SIZE(sunxi_codec_controls));
	gpio_set_value(pa_item.gpio.gpio, 1);
	msleep(20);
	/*auto-detect fs*/
	snd_soc_write(codec, 0x1b, 0x00);
	msleep(50);
	ret = drc_init();
	if (ret < 0)
		return ret;

	ret = bd_mode_init();
	if (ret < 0)
		return ret;

	snd_soc_write(codec, 0x11, 0xb8);
	snd_soc_write(codec, 0x12, 0x60);
	snd_soc_write(codec, 0x13, 0xa0);
	snd_soc_write(codec, 0x14, 0x48);
	for (i = 0; ARRAY_SIZE(tas5707_vol_table); i++) {
		if (tas5707_vol_table[i].vtable_step == vol_step) {
			snd_soc_write(g_codec,\
				0x07, tas5707_vol_table[i].vol_reg);
			break;
		}
	}
	msleep(20);
	/* EQ */
	ret = set_eq_config();
	if (ret < 0)
		return ret;

	gpio_set_value(pa_item.gpio.gpio, 0);
	INIT_WORK(&tas5707->linein_jack, tas5707_linein_work);

	ret = snd_soc_jack_new(tas5707->codec, "sunxi linein Jack",\
			SND_JACK_LINEIN_SIGNAL | SND_JACK_LINEIN\
			| SND_JACK_LINEIN_PLUGOUT, &tas5707->jack);
	if (ret) {
		pr_err("%s,l:%d,jack creation failed\n", __func__, __LINE__);
		return ret;
	}
	return 0;
}

static int tas5707_remove(struct snd_soc_codec *codec)
{
	struct tas5707_priv *codectas5707 = snd_soc_codec_get_drvdata(codec);

	kfree(codectas5707);

	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_tas5707 = {
	.probe				= tas5707_probe,
	.remove				= tas5707_remove,
	.suspend			= tas5707_suspend,
	.resume				= tas5707_resume,
	.ignore_pmdown_time = 1,
};

/*****************************************************/
static int __devinit tas5707_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *i2c_id)
{
	int ret = 0;
	int req_status = 0;
	struct tas5707_priv *tas5707;
	script_item_value_type_e  type;

	tas5707_client = i2c;

	tas5707 = devm_kzalloc(&i2c->dev, sizeof(struct tas5707_priv),
			      GFP_KERNEL);
	if (tas5707 == NULL) {
		dev_err(&i2c->dev, "Unable to allocate private data\n");
		return -ENOMEM;
	} else {
		dev_set_drvdata(&i2c->dev, tas5707);
	}
	tas5707->i2c = i2c;

	/*set power*/
	type = script_get_item("tas5707", "chip_en", &chip_en);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("[ audio ] err:try to get chip_en failed!\n");
		return -EFAULT;
	}
	/*request gpio*/
	req_status = gpio_request(chip_en.gpio.gpio, NULL);
	if (0 != req_status)
		pr_err("request gpio failed!\n");
	gpio_direction_output(chip_en.gpio.gpio, 1);
	gpio_set_value(chip_en.gpio.gpio, 1);

	 /*set power*/
	type = script_get_item("tas5707", "tas5707_power0", &tas5707_power0);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("[ audio ] err:try to get tas5707_power0 failed!\n");
		return -EFAULT;
	}
	/*request gpio*/
	req_status = gpio_request(tas5707_power0.gpio.gpio, NULL);
	if (0 != req_status)
		pr_err("request gpio failed!\n");
	gpio_direction_output(tas5707_power0.gpio.gpio, 1);
	gpio_set_value(tas5707_power0.gpio.gpio, 1);

	/*set power*/
	type = script_get_item("tas5707", "tas5707_power1", &tas5707_power1);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("[ audio ] err:try to get tas5707_power1 failed!\n");
		return -EFAULT;
	}
	/*request gpio*/
	req_status = gpio_request(tas5707_power1.gpio.gpio, NULL);
	if (0 != req_status)
		pr_err("request gpio failed!\n");
	gpio_direction_output(tas5707_power1.gpio.gpio, 1);
	gpio_set_value(tas5707_power1.gpio.gpio, 1);

	/*get the default pa ctl(close)*/
	type = script_get_item("tas5707", "amp_pa_ctrl", &pa_item);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("[ audio ] err:try to get audio_pa_ctrl failed!\n");
		return -EFAULT;
	}
	/*request gpio*/
	req_status = gpio_request(pa_item.gpio.gpio, NULL);
	if (0 != req_status)
		pr_err("request gpio failed!\n");
	gpio_direction_output(pa_item.gpio.gpio, 1);

	gpio_set_value(pa_item.gpio.gpio, 0);

	/* get config of tas5707 codec reset gpio pin*/
	type = script_get_item("tas5707", "tas5707_reset", &reset_item);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("cx_codec tas5707_reset script_get_item return type err\n");
	} else {
		/*request tas5707_reset codec reset control gpio*/
		ret = gpio_request(reset_item.gpio.gpio, NULL);
		if (0 != ret) {
			pr_err("request tas5707_reset gpio failed!\n");
		} else {
			/*
			* config gpio as output.
			*/
			gpio_direction_output(reset_item.gpio.gpio, 1);
			gpio_set_value(reset_item.gpio.gpio, 1);
			/* reset the tas5707_reset codec*/
			gpio_set_value(reset_item.gpio.gpio, 0);
			msleep(20);
			gpio_set_value(reset_item.gpio.gpio, 1);
		}
	}

	type = script_get_item("tas5707", "aux_signal", &g_aux_signal_item);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("[ audio ] err:try to get aux_signal failed!\n");
		return -EFAULT;
	}
	/*request g_aux_signal_item gpio*/
	req_status = gpio_request((g_aux_signal_item.gpio.gpio), NULL);
	if (0 != req_status)
		pr_err("request g_aux_signal_item gpio failed!\n");
	gpio_direction_input(g_aux_signal_item.gpio.gpio);
	master_vol = 0;

	if (i2c_id->driver_data == 0)
		ret = snd_soc_register_codec(&i2c->dev, &soc_codec_dev_tas5707,\
				&tas5707_dai0, 1);
	else
		pr_err("The wrong i2c_id number :%ld\n", i2c_id->driver_data);

	return ret;
}

static __devexit int tas5707_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);

	return 0;
}

static struct i2c_board_info tas5707_i2c_board_info[] = {
	/*
	* 1.the 7-bit address for TAS5707A is 0011 101 (0x3A)
	* so the i2c devices address of TAS5707A is 0x1d (0x3a = 2 * 0x1d)
	* 2.the 7-bit address for TAS5707 is 0011 101 (0x36)
	* so the i2c devices address of TAS5707 is 0x1b (0x36 = 2 * 0x1b)
	*/
	{I2C_BOARD_INFO("tas5707_0", 0x1b),	},
};

static const struct i2c_device_id tas5707_i2c_id[] = {
	{ "tas5707_0", 0 },
};
MODULE_DEVICE_TABLE(i2c, tas5707_i2c_id);

static struct i2c_driver tas5707_i2c_driver = {
	.driver = {
		.name = "tas5707",
		.owner = THIS_MODULE,
	},
	.probe = tas5707_i2c_probe,
	.remove = __devexit_p(tas5707_i2c_remove),
	.id_table = tas5707_i2c_id,
};

static int __init tas5707_init(void)
{
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	int i = 0;

	script_item_u val;
	script_item_value_type_e  type;
	tas5707_used = 0;
	g_aux_nosignal_count = 0;
	g_aux_signal_count = 0;

	AUX_JACK_DETECT		= false;
	AUX_SIGNAL_DETECT	= false;

	tas5707_linein_en	= false;
	tas5707_playing_en	= false;
	aux_signal			= false;
	type = script_get_item("tas5707", "tas5707_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("tas5707 type err!\n");
	else
		tas5707_used = val.val;

	if (tas5707_used) {
		adapter = i2c_get_adapter(I2C_CHANNEL_NUM);
		if (!adapter)
			return -ENODEV;
		for (i = 0; i < sizeof(tas5707_i2c_board_info)/\
			sizeof(tas5707_i2c_board_info[0]); i++) {
			client = NULL;
			client = i2c_new_device(adapter,\
					&tas5707_i2c_board_info[i]);
			if (!client)
				return -ENODEV;
		}
		i2c_put_adapter(adapter);

		return i2c_add_driver(&tas5707_i2c_driver);
	} else {
		return 0;
	}
}
module_init(tas5707_init);
module_param_named(AUX_JACK_DETECT, AUX_JACK_DETECT, bool, S_IRUGO | S_IWUSR);
module_param_named(AUX_SIGNAL_DETECT, AUX_SIGNAL_DETECT, bool,\
					S_IRUGO | S_IWUSR);

static void __exit tas5707_exit(void)
{
	i2c_del_driver(&tas5707_i2c_driver);
}
module_exit(tas5707_exit);

MODULE_DESCRIPTION("ASoC codec tas5707 driver");
MODULE_AUTHOR("huangxin <huangxin@allwinnertech.com>");
MODULE_LICENSE("GPL");
