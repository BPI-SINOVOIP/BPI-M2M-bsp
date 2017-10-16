/*
 * sound\soc\sunxi\sunxi_sndcodec.c
 * (C) Copyright 2014-2017
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * guoyingyang <guoyingyang@allwinnertech.com>
 *
 * some simple description for this code
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/input.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/jack.h>
#include <linux/of.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include "sun8iw5_sndcodec.h"
#include <linux/delay.h>
#include <mach/sys_config.h>
#include <linux/pinctrl/consumer.h>

static struct snd_soc_dai *card0_device0_interface;

/*add a headphone jack*/
static struct snd_soc_jack jack;



/*add headpone jack detection DAPM pins*/
static struct snd_soc_jack_pin jack_pins[] = {
	{
		.pin = "Linein_detect",
		.mask = SND_JACK_LINEIN,
	}
};

/* add headphone jack detection gpios */
static struct snd_soc_jack_gpio jack_gpios[] = {
	{
		.name = "SUNXI_Linein_detect",
		.report = SND_JACK_LINEIN,
		.debounce_time = 200,
		.invert = 1
	}
};



struct mc_private {
	struct snd_soc_codec *codec;
	script_item_u audio_linein_detect;
	u32 aif2master;
	u32 aif2fmt;
	u32 aif3fmt;
};

static const struct snd_kcontrol_new ac_pin_controls[] = {
	SOC_DAPM_PIN_SWITCH("Headphone"),
	SOC_DAPM_PIN_SWITCH("Linein_detect"),
};

static const struct snd_soc_dapm_widget sunxi_ac_dapm_widgets[] = {
	SND_SOC_DAPM_MIC("External MainMic", NULL),
	SND_SOC_DAPM_MIC("HeadphoneMic", NULL),
	SND_SOC_DAPM_LINE("Linein_detect", NULL),
};

static const struct snd_soc_dapm_route audio_map[] = {
	{"MainMic Bias", NULL, "External MainMic"},
	{"MIC1P", NULL, "MainMic Bias"},
	{"MIC1N", NULL, "MainMic Bias"},

	{"MIC2", NULL, "HeadphoneMic"},
	{"LINEINL", NULL, "Linein_detect"},
	{"LINEINR", NULL, "Linein_detect"},
};

/*
 * Card initialization
 */
static int sunxi_audio_init(struct snd_soc_pcm_runtime *runtime)
{
	int ret;
	struct snd_soc_codec *codec = runtime->codec;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct mc_private *ctx = snd_soc_card_get_drvdata(runtime->card);

	card0_device0_interface = runtime->cpu_dai;
	ctx->codec = runtime->codec;

	snd_soc_dapm_disable_pin(&codec->dapm,	"HPOUTR");
	snd_soc_dapm_disable_pin(&codec->dapm,	"HPOUTL");
	snd_soc_dapm_disable_pin(&runtime->card->dapm,	"Headphone");
	snd_soc_dapm_disable_pin(&runtime->card->dapm,	"Linein_detect");

	if (gpio_is_valid(jack_gpios[0].gpio)) {
		ret = snd_soc_jack_new(codec, "Linein Jack", SND_JACK_LINEIN | SND_JACK_LINEIN_PLUGOUT, &jack);
		if (ret) {
			pr_err("can not creat jack for Linein \n");
			return ret ;
		}
		ret = snd_soc_jack_add_pins(&jack, ARRAY_SIZE(jack_pins), jack_pins);
		if (ret) {
			pr_err(" snd_soc_jack_add_pins error\n");
			return ret;
		}
		ret = snd_soc_jack_add_gpios(&jack, ARRAY_SIZE(jack_gpios), jack_gpios);
		if (ret) {
			pr_err("snd_soc_jack_add_gpios error\n");
			return ret;
		}
	}

	snd_soc_dapm_sync(dapm);
	return 0;
}


static int sunxi_sndpcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	int ret  = 0;
	u32 freq_in = 22579200;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
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
		freq_in = 24576000;
		break;
	}
	/* set the codec FLL */
	ret = snd_soc_dai_set_pll(codec_dai, PLLCLK, 0, freq_in, freq_in);
	if (ret < 0) {
		pr_err("err:%s,set codec dai pll failed.\n", __func__);
		return ret;
	}
	/*set system clock source freq */
	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, freq_in, 0);
	if (ret < 0) {
		pr_err("err:%s,set cpu dai sysclk failed.\n", __func__);
		return ret;
	}
	/*set system clock source freq_in and set the mode as tdm or pcm*/
	ret = snd_soc_dai_set_sysclk(codec_dai, AIF1_CLK, freq_in, 0);
	if (ret < 0) {
		pr_err("err:%s,set codec dai sysclk faided.\n", __func__);
		return ret;
	}
	/*set system fmt:api2s:master aif1:slave*/
	ret = snd_soc_dai_set_fmt(cpu_dai, 0);
	if (ret < 0) {
		pr_err("%s,set cpu dai fmt failed.\n", __func__);
		return ret;
	}

	/*
	* codec: slave. AP: master
	*/
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s,set codec dai fmt failed.\n", __func__);
		return ret;
	}

	ret = snd_soc_dai_set_clkdiv(cpu_dai, 0, sample_rate);
	if (ret < 0) {
		pr_err("%s, set cpu dai clkdiv faided.\n", __func__);
		return ret;
	}

	return 0;
}


static struct snd_soc_ops sunxi_sndpcm_ops = {
       .hw_params              = sunxi_sndpcm_hw_params,
};

static int aif2_clk_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_card *card = rtd->card;
	struct mc_private *ctx = NULL;

	int ret = 0;
	int freq_in = 24576000;
	if (params_rate(params) != 8000)
		pr_warning("%s,line:%d,params_rate(params):%d\n", __func__, __LINE__, params_rate(params));

	ctx = snd_soc_card_get_drvdata(card);
	if (ctx == NULL) {
		pr_err("err:%s,get ctx failed.\n", __func__);
		return -EINVAL;
	}
	/*set system clock source freq */
	ret = snd_soc_dai_set_sysclk(card0_device0_interface, 0, freq_in, 0);
	if (ret < 0) {
		pr_err("err:%s,set cpu dai sysclk failed.\n", __func__);
		return ret;
	}
	/* set the codec aif1clk/aif2clk from pllclk */
	ret = snd_soc_dai_set_pll(codec_dai, PLLCLK, 0, freq_in, freq_in);
	if (ret < 0) {
		pr_err("err:%s,set codec dai pll failed.\n", __func__);
		return ret;
	}
	/*set system clock source aif2*/
	ret = snd_soc_dai_set_sysclk(codec_dai, AIF2_CLK , 0, 0);
	if (ret < 0) {
		pr_err("err:%s,set codec dai sysclk faied\n", __func__);
		return ret;
	}
	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_DSP_A | (ctx->aif2fmt << 8) | (ctx->aif2master << 12));
	if (ret < 0) {
		pr_err("err:%s,set codec dai fmt failed.\n", __func__);
	}
	return ret;
}


static struct snd_soc_ops aif2_clk_ops = {
	.hw_params = aif2_clk_hw_params,
};

static int aif3_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params)
{
	struct mc_private *ctx = NULL;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_card *card = rtd->card;
	int ret = 0;
	if (params_rate(params) != 8000)
		pr_warning("%s,line:%d,params_rate(params):%d\n", __func__, __LINE__, params_rate(params));
	ctx = snd_soc_card_get_drvdata(card);
	if (ctx == NULL) {
		pr_err("err:%s,get ctx failed.\n", __func__);
		return -EINVAL;
	}
	/* set codec aif3 configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, (ctx->aif3fmt << 8));
	if (ret < 0)
		return ret;
	return 0;
}


static struct snd_soc_ops aif3_clk_ops = {
	.hw_params = aif3_hw_params,
};

static struct snd_soc_dai_link sunxi_sndpcm_dai_link[] = {
	{
		.name = "audiocodec",
		.stream_name 	= "SUNXI-CODEC",
		.cpu_dai_name 	= "sunxi-codec",
		.codec_dai_name = "codec-aif1",
		.platform_name 	= "sunxi-pcm-codec-audio",
		.codec_name 	= "sunxi-internal-codec",
		.init 			= sunxi_audio_init,
		.ops = &sunxi_sndpcm_ops,
	},
	/*digital */
	{
		.name = "codec_aif2",
		.stream_name = "aif2-clk",
		.cpu_dai_name = "sunxi-codec-aif2-dai",
		.codec_dai_name = "codec-aif2",
		.codec_name = "sunxi-internal-codec",
		.ops = &aif2_clk_ops,
	},
	{
		.name = "codec_aif3",
		.stream_name = "aif3_clk",
		.cpu_dai_name = "sunxi-codec-aif2-dai",
		.codec_dai_name = "codec-aif3",
		.codec_name = "sunxi-internal-codec",
		.ops = &aif3_clk_ops,
	},
};

static int jack_gpio_request(u32 gpio, bool enable)
{
	char pin_name[8];
	u32 config, ret;
	sunxi_gpio_to_name(gpio, pin_name);
	if (!enable)
		config = (((7) << 16) | (0 & 0xFFFF));
	else
		config = (((6) << 16) | (0 & 0xFFFF));
	ret = pin_config_set(SUNXI_PINCTRL, pin_name, config);
	return ret;
}


static int sunxi_suspend(struct snd_soc_card *card)
{
	if (gpio_is_valid(jack_gpios[0].gpio)) {
		disable_irq(gpio_to_irq(jack_gpios[0].gpio));
		jack_gpio_request(jack_gpios[0].gpio, 0);
	}
	return 0;
}

static int sunxi_resume(struct snd_soc_card *card)
{
	if (gpio_is_valid(jack_gpios[0].gpio)) {
		jack_gpio_request(jack_gpios[0].gpio, 1);
		schedule_delayed_work(&jack_gpios[0].work, msecs_to_jiffies(jack_gpios[0].debounce_time));
		enable_irq(gpio_to_irq(jack_gpios[0].gpio));
	}
	return 0;
}


static struct snd_soc_card snd_soc_sunxi_sndpcm = {
	.name 		= "sndcodec",
	.owner 		= THIS_MODULE,
	.dai_link 	= sunxi_sndpcm_dai_link,
	.num_links 	= ARRAY_SIZE(sunxi_sndpcm_dai_link),
	.dapm_widgets = sunxi_ac_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(sunxi_ac_dapm_widgets),
	.dapm_routes = audio_map,
	.num_dapm_routes = ARRAY_SIZE(audio_map),
	.controls = ac_pin_controls,
	.num_controls = ARRAY_SIZE(ac_pin_controls),
	.suspend_post = sunxi_suspend,
	.resume_post = sunxi_resume,
};


static int __devinit sunxi_sndpcm_dev_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct mc_private *ctx = NULL;
	script_item_value_type_e  type;
	script_item_u val;
	script_item_u linein_item;
	struct snd_soc_card *card = &snd_soc_sunxi_sndpcm;

	card->dev = &pdev->dev;

	ctx = devm_kzalloc(&pdev->dev, sizeof(struct mc_private), GFP_KERNEL);
	if (!ctx) {
		pr_err("[audio] allocation mem failed\n");
		return -ENOMEM;
	}
	type = script_get_item("codec_aif2", "aif2fmt", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[machine] aif2fmt type err!\n");
	} else {
		ctx->aif2fmt = val.val;
	}
	type = script_get_item("codec_aif2", "aif2master", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[machine] aif2master type err!\n");
	} else {
		ctx->aif2master = val.val;
	}

	type = script_get_item("codec_aif3", "aif3fmt", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[machine] aif3fmt type err!\n");
	} else {
		ctx->aif3fmt = val.val;
	}

	type = script_get_item("audio0", "audio_linein_detect", &linein_item);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		jack_gpios[0].gpio = -1;
		pr_err("[ audio ] err:try to get audio_linein_detect failed!\n");
	} else{
		jack_gpios[0].gpio = linein_item.gpio.gpio;
		pr_debug("jack_gpio num is %d \n", jack_gpios[0].gpio);
	}

	snd_soc_sunxi_sndpcm.dev = &pdev->dev;

	snd_soc_card_set_drvdata(&snd_soc_sunxi_sndpcm, ctx);

	platform_set_drvdata(pdev, &snd_soc_sunxi_sndpcm);

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
			ret);
	}
	return ret;
}

static int __devexit sunxi_sndpcm_dev_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	snd_soc_unregister_card(card);
	return 0;
}


/*data relating*/
static struct platform_device sunxi_sndpcm_device = {
	.name 	= "audiocodec",
	.id 	= PLATFORM_DEVID_NONE,
};


/*method relating*/
static struct platform_driver sunxi_sndpcm_driver = {
	.probe = sunxi_sndpcm_dev_probe,
	.remove = __exit_p(sunxi_sndpcm_dev_remove),
	.driver = {
		.name = "audiocodec",
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
	},
};


static int __init sunxi_snddaudio0_init(void)
{
	int err = 0;
	script_item_u val;
	script_item_value_type_e  type;
	int codec_used = 0;
	type = script_get_item("audio0", "audio_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[R16] R16 codec used type err!\n");
	}
	codec_used = val.val;
	if (codec_used) {
		err = platform_device_register(&sunxi_sndpcm_device);
		if (err < 0)
			return err;
		err = platform_driver_register(&sunxi_sndpcm_driver);
		if (err < 0)
			return err;
	}
	return 0;
}
module_init(sunxi_snddaudio0_init);

static void __exit sunxi_snddaudio0_exit(void)
{
	platform_driver_unregister(&sunxi_sndpcm_driver);
}
module_exit(sunxi_snddaudio0_exit);


MODULE_AUTHOR("guoyingyang");
MODULE_DESCRIPTION("SUNXI R16 alsa machine driver");
MODULE_LICENSE("GPL");
