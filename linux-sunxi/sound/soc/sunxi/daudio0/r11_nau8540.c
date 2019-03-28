/*
 * sound\soc\sunxi\daudio0\r11_nau8540.c
 * (C) Copyright 2014-2018
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Author: linjunyan <junyanlin@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include <linux/io.h>
#include <mach/sys_config.h>
#include "sunxi-daudio0.h"
#include "sunxi-daudiodma0.h"
#include "../../codecs/nau8540.h"

#define NAU8540_NAME "nau8540"

static bool daudio_pcm_select;
static int daudio_used;
static int daudio_master;
static int audio_format;
static int signal_inversion;

struct nau8540_config {
	int used;
	int mic1_input_used;
	int mic2_input_used;
	int mic3_input_used;
	int mic4_input_used;
	int data_output_port; /* 0: DO12, 1: DO34 */
	int digital_audio_mode; /* 0: normal mode (stereo), 1: TDM mode */
};

static struct nau8540_config nau8540_config = {
	.used = 0,
	.mic1_input_used = 0,
	.mic2_input_used = 0,
	.mic3_input_used = 0,
	.mic4_input_used = 0,
	.data_output_port = 0,
	.digital_audio_mode = 0,
};

static int sunxi_snddaudio_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	int ret  = 0;
	u32 freq = 22579200;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned long sample_rate = params_rate(params);

	switch (sample_rate) {
	case 8000:
	case 16000:
	case 32000:
	case 64000:
	case 128000:
	case 12000:
	case 24000:
	case 48000:
	case 96000:
	case 192000:
		freq = 24576000;
		break;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, freq, daudio_pcm_select);
	if (ret < 0) {
		pr_err("%s, line %d: err\n",
				__func__, __LINE__);
		return ret;
	}
	ret = snd_soc_dai_set_clkdiv(cpu_dai, 0, sample_rate);
	if (ret < 0) {
		pr_err("%s, line %d: err\n", __func__, __LINE__);
		return ret;
	}
	ret = snd_soc_dai_set_fmt(cpu_dai, audio_format \
			| (signal_inversion << 8) | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s, line %d: err\n", __func__, __LINE__);
		return ret;
	}
	ret = snd_soc_dai_set_fmt(codec_dai, audio_format \
			| (signal_inversion << 8) | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s, line %d: err\n", __func__, __LINE__);
		return ret;
	}
	ret = snd_soc_dai_set_pll(codec_dai, NAU8540_CLK_FLL_FS, 0,
			sample_rate, sample_rate * 256);
	if (ret < 0) {
		pr_err("%s, line %d: err\n", __func__, __LINE__);
		return ret;
	}

	/*
	 * TODO: add support for TDM mode. Currently the code only supports
	 * normal mode (stereo).
	 */

	return 0;
}

static struct snd_soc_ops sunxi_snddaudio_ops = {
	.hw_params	= sunxi_snddaudio_hw_params,
};

static int sunxi_daudio_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dapm_context *dapm = &rtd->card->dapm;
	struct snd_soc_codec *codec = rtd->codec;
	script_item_u val;
	script_item_value_type_e type;
	unsigned int reg_value;
	int err;

	type = script_get_item(NAU8540_NAME, "mic1_input_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] mic1_input_used type err!\n", NAU8540_NAME);
	nau8540_config.mic1_input_used = val.val;
	if (!nau8540_config.mic1_input_used)
		snd_soc_dapm_disable_pin(dapm, "MIC1");

	type = script_get_item(NAU8540_NAME, "mic2_input_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] mic2_input_used type err!\n", NAU8540_NAME);
	nau8540_config.mic2_input_used = val.val;
	if (!nau8540_config.mic2_input_used)
		snd_soc_dapm_disable_pin(dapm, "MIC2");

	type = script_get_item(NAU8540_NAME, "mic3_input_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] mic3_input_used type err!\n", NAU8540_NAME);
	nau8540_config.mic3_input_used = val.val;
	if (!nau8540_config.mic3_input_used)
		snd_soc_dapm_disable_pin(dapm, "MIC3");

	type = script_get_item(NAU8540_NAME, "mic4_input_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] mic4_input_used type err!\n", NAU8540_NAME);
	nau8540_config.mic4_input_used = val.val;
	if (!nau8540_config.mic4_input_used)
		snd_soc_dapm_disable_pin(dapm, "MIC4");

	type = script_get_item(NAU8540_NAME, "data_output_port", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] data_output_por type err!\n", NAU8540_NAME);
	nau8540_config.data_output_port = val.val;
	if (nau8540_config.data_output_port == 0) {
		reg_value = codec->read(codec, NAU8540_REG_PCM_CTRL1);
		err = codec->write(codec, NAU8540_REG_PCM_CTRL1,
				reg_value &= ~(NAU8540_I2S_DO12_TRI));
		if (err < 0)
			pr_err("[nau8540] Failed to enable DO12\n");
	} else {
		reg_value = codec->read(codec, NAU8540_REG_PCM_CTRL2);
		err = codec->write(codec, NAU8540_REG_PCM_CTRL2,
				reg_value &= ~(NAU8540_I2S_DO34_TRI));
		if (err < 0)
			pr_err("[nau8540] Failed to enable DO34\n");
	}

	type = script_get_item(NAU8540_NAME, "digital_audio_mode", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] digital_audio_mode type err!\n", NAU8540_NAME);
	nau8540_config.digital_audio_mode = val.val;

	return 0;
}

static struct snd_soc_dai_link sunxi_snddaudio_dai_link = {
	.name		= TDM_NAME,
	.cpu_dai_name 	= "pri_dai",
	.stream_name 	= "SUNXI-TDM0",
	.codec_dai_name	= "nau8540-hifi",
	.codec_name	= "nau8540.1-001d",
	.init		= sunxi_daudio_init,
	.platform_name	= "sunxi-daudio-pcm-audio.0",
	.ops		= &sunxi_snddaudio_ops,
};

static struct snd_soc_card snd_soc_sunxi_snddaudio = {
	.name		= "snddaudio",
	.owner		= THIS_MODULE,
	.dai_link	= &sunxi_snddaudio_dai_link,
	.num_links	= 1,
};

static int __devinit sunxi_snddaudio0_dev_probe(struct platform_device *pdev)
{
	int ret = 0;
	script_item_u val;
	script_item_value_type_e  type;
	struct snd_soc_card *card = &snd_soc_sunxi_snddaudio;

	type = script_get_item(TDM_NAME, "daudio_select", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] daudio_select type err!\n", TDM_NAME);
	daudio_pcm_select = val.val;

	type = script_get_item(TDM_NAME, "daudio_master", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] daudio_master type err!\n", TDM_NAME);
	daudio_master = val.val;

	type = script_get_item(TDM_NAME, "audio_format", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] audio_format type err!\n", TDM_NAME);
	audio_format = val.val;

	type = script_get_item(TDM_NAME, "signal_inversion", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s] signal_inversion type err!\n", TDM_NAME);
	signal_inversion = val.val;

	card->dev = &pdev->dev;
	ret = snd_soc_register_card(card);
	if (ret)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);
	return ret;
}

static int __devexit sunxi_snddaudio0_dev_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	snd_soc_unregister_card(card);
	return 0;
}

static struct platform_device sunxi_daudio_device = {
	.name	= "snddaudio",
	.id	= PLATFORM_DEVID_NONE,
};

static struct platform_driver sunxi_daudio_driver = {
	.probe = sunxi_snddaudio0_dev_probe,
	.remove = __exit_p(sunxi_snddaudio0_dev_remove),
	.driver = {
		.name = "snddaudio",
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
	},
};

static int __init sunxi_snddaudio0_init(void)
{
	int err = 0;
	script_item_u val;
	script_item_value_type_e  type;

	type = script_get_item(TDM_NAME, "daudio_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type)
		pr_err("[%s]: daudio_used type err!\n", TDM_NAME);
	daudio_used = val.val;

	type = script_get_item(NAU8540_NAME, "nau8540_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT !=  type)
		pr_err("[%s]: nau8540_used type err!\n", NAU8540_NAME);
	nau8540_config.used = val.val;

	if (daudio_used && nau8540_config.used) {
		err = platform_device_register(&sunxi_daudio_device);
		if (err < 0)
			return err;
		err = platform_driver_register(&sunxi_daudio_driver);
		if (err < 0)
			return err;
	} else {
		pr_warning("[DAUDIO0] driver not init, just return.\n");
	}

	return 0;
}
module_init(sunxi_snddaudio0_init);

static void __exit sunxi_snddaudio0_exit(void)
{
	if (daudio_used && nau8540_config.used) {
		daudio_used = 0;
		nau8540_config.used = 0;
		platform_driver_unregister(&sunxi_daudio_driver);
		platform_device_unregister(&sunxi_daudio_device);
	}
}
module_exit(sunxi_snddaudio0_exit);

MODULE_AUTHOR("linjunyan");
MODULE_DESCRIPTION("SUNXI R11-NAU8520/40 machine driver");
MODULE_LICENSE("GPL");
