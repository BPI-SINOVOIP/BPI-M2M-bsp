/*
 * sound\soc\sunxi\audiocodec\sunxi_sndcodec.c
 * (C) Copyright 2010-2016
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * huangxin <huangxin@Reuuimllatech.com>
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
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <mach/sys_config.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/jack.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>

#define AUDIOCODEC_GPIO

#ifdef CONFIG_ARCH_SUN8IW9
#include "sun8iw9_sndcodec.h"
#ifdef AUDIOCODEC_GPIO
static script_item_u item;
#endif

#undef HS_DBG
#if (1)
    #define HS_DBG(format,args...)  printk("[SWITCH] "format,##args)
#else
    #define HS_DBG(...)
#endif

#define SUN8IW9_INTR_DEBOUNCE               200
#define SUN8IW9_HS_INSERT_DET_DELAY         500
#define SUN8IW9_HS_REMOVE_DET_DELAY         500
#define SUN8IW9_BUTTON_DET_DELAY            100
#define SUN8IW9_HS_DET_POLL_INTRVL          100
#define SUN8IW9_BUTTON_EN_DELAY             1500
#define SUN8IW9_HS_DET_RETRY_COUNT          6

static 	int	JACK_IN_IRQ_EN = 4;
static 	int	JACK_OUT_IRQ_EN = 3;
static 	int	JACK_DET_OUT_ST = 4;
static 	int	JACK_DET_IIN_ST = 3;

static bool is_irq = false;
static int  switch_state = 0;
static int audio_used = 0;
typedef enum {
	RESUME_IRQ = 0x0,
	SYSINIT_IRQ = 0x1,
	OTHER_IRQ = 0x2,
} _jack_irq_mode;

enum HPDETECTWAY {
	HP_DETECT_LOW = 0x0,
	HP_DETECT_HIGH = 0x1,
};

struct sun8iw9_mc_private {
	struct snd_soc_jack jack;
	struct delayed_work hs_init_work;
	struct delayed_work hs_insert_work;
	struct delayed_work hs_remove_work;
	struct delayed_work hs_button_work;
	struct mutex jack_mlock;
	struct snd_soc_codec *codec;
	/* To enable button press interrupts after a delay after HS detection.
	 * This is to avoid spurious button press events during slow HS insertion
	 */
	struct delayed_work hs_button_en_work;
	struct timeval tv_headset_plugin;/*4*/

	int state;	/*switch state: headphone,headset,idle */
	int jackirq;	/*switch irq*/
	int HEADSET_DATA;	/*threshod for switch insert*/
	int headset_basedata;
	int hp_detect_case;
	int intr_debounce;
	int hs_insert_det_delay;
	int hs_remove_det_delay;
	int button_det_delay;
	int button_en_delay;
	int hs_det_poll_intrvl;
	int hs_det_retry;
	int switch_status;
	int key_volup;
	int key_voldown;
	int key_hook;
	bool process_button_events;
	_jack_irq_mode jack_irq_times;
};

/* Identify the jack type as Headset/Headphone/None */
static int sunxi_check_jack_type(struct snd_soc_jack *jack)
{
	u32 reg_val;
	u32 jack_type = 0,tempdata = 0,flag = 0;
	struct sun8iw9_mc_private *ctx = container_of(jack, struct sun8iw9_mc_private, jack);

	reg_val  = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
	tempdata = (reg_val>>HMIC_DATA)&0x1f;

	while (tempdata >= 0x1D && flag != 20) {
		msleep(45);
		tempdata = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
		tempdata &=(0x1f<<8);
		tempdata = tempdata>>8;
		flag++;
	}

	if (flag == 20 ) {
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x0<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x0<<MICADCEN));
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x1<<JACK_IN_IRQ_EN));
		jack_type = 0;
		return jack_type;
	}

	pr_debug("tempdata:%x,ctx->HEADSET_DATA:%x,reg_val:%x\n",tempdata,ctx->HEADSET_DATA,reg_val);

	ctx->headset_basedata = tempdata;
	if (tempdata >= ctx->HEADSET_DATA) {/*headset:4*/
		jack_type = SND_JACK_HEADSET;
	} else {/*headphone:3*/
		/*disable hbias and adc*/
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x0<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x0<<MICADCEN));
		jack_type = SND_JACK_HEADPHONE;
	}
	return jack_type;
}

/* Checks jack insertion and identifies the jack type.*/
static void sunxi_check_hs_insert_status(struct work_struct *work)
{
	struct sun8iw9_mc_private *ctx = container_of(work, struct sun8iw9_mc_private, hs_insert_work.work);
	int jack_type = 0,reg_val = 0;

	mutex_lock(&ctx->jack_mlock);
	jack_type = sunxi_check_jack_type(&ctx->jack);
	if (jack_type != ctx->switch_status) {
		ctx->switch_status = jack_type;
		snd_jack_report(ctx->jack.jack, jack_type);
		pr_debug("switch:%d\n",jack_type);
		switch_state = jack_type;
	}

	/*if SND_JACK_HEADSET,enable mic detect irq*/
	if (jack_type == SND_JACK_HEADSET) {/*4*/
		/*clear headset insert pending.*/
		reg_val = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
		ctx->headset_basedata = (reg_val>>HMIC_DATA)&0x1f;
		ctx->headset_basedata -= 2;

		do_gettimeofday(&ctx->tv_headset_plugin);
		usleep_range(1000, 2000);
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0x1f<<MDATA_THRESHOLD), (0x14<<MDATA_THRESHOLD));
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x1<<MIC_DET_IRQ_EN));
	} else if (jack_type == SND_JACK_HEADPHONE) {
		/*if is HEADPHONE 3,close mic detect irq*/
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x0<<MIC_DET_IRQ_EN));
		ctx->tv_headset_plugin.tv_sec = 0;
	}
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x1<<JACK_OUT_IRQ_EN));

	mutex_unlock(&ctx->jack_mlock);
}
static void sunxi_hs_init_work(struct work_struct *work)
{
	struct sun8iw9_mc_private *ctx = container_of(work, struct sun8iw9_mc_private, hs_init_work.work);
	int tempdata;

	mutex_lock(&ctx->jack_mlock);
	if (is_irq == true) {
		is_irq = false;
	} else {
		if (ctx->hp_detect_case == HP_DETECT_HIGH) {/*yj*/
			tempdata =snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
			tempdata = (tempdata&0x1f00)>>8;
			if (tempdata < 0x5) {
				ctx->switch_status = SND_JACK_HEADPHONE;
				snd_jack_report(ctx->jack.jack, SND_JACK_HEADPHONE);
				snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x0<<HMICBIASEN));
				snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x0<<MICADCEN));
				snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x0<<MIC_DET_IRQ_EN));
				switch_state = SND_JACK_HEADPHONE;
			} else {
				ctx->switch_status = SND_JACK_HEADSET;
				snd_jack_report(ctx->jack.jack, SND_JACK_HEADSET);
				switch_state = SND_JACK_HEADSET;
				msleep(500);
				snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0x1f<<MDATA_THRESHOLD), (0x14<<MDATA_THRESHOLD));
				snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x1<<MIC_DET_IRQ_EN));
			}
		}

		if (ctx->hp_detect_case == HP_DETECT_LOW) {
			ctx->switch_status = 0;
			snd_jack_report(ctx->jack.jack, ctx->switch_status);
			switch_state = 0;
		}

	}
	ctx->jack_irq_times = OTHER_IRQ;

	mutex_unlock(&ctx->jack_mlock);
}

/* Check for hook release */
static void sunxi_check_hs_button_status(struct work_struct *work)
{
	struct sun8iw9_mc_private *ctx = container_of(work, struct sun8iw9_mc_private, hs_button_work.work);
	u32 i = 0;
	mutex_lock(&ctx->jack_mlock);
	for (i = 0;i < 5; i++){
		if (ctx->key_hook == 0 ){
			pr_debug("Hook (2)!!\n");
			ctx->switch_status &= ~SND_JACK_BTN_0;
			snd_jack_report(ctx->jack.jack, ctx->switch_status);
			break;
		}
		msleep(8);
	}
	mutex_unlock(&ctx->jack_mlock);
}

/* Checks jack removal. */
static void sunxi_check_hs_remove_status(struct work_struct *work)
{
	struct sun8iw9_mc_private *ctx = container_of(work, struct sun8iw9_mc_private, hs_remove_work.work);
	mutex_lock(&ctx->jack_mlock);
	ctx->switch_status = 0;
	/*clear headset pulgout pending.*/
	snd_jack_report(ctx->jack.jack, ctx->switch_status);
	switch_state = ctx->switch_status;
	pr_debug("switch:%d\n",ctx->switch_status);
	ctx->tv_headset_plugin.tv_sec = 0;

	mutex_unlock(&ctx->jack_mlock);
}

static irqreturn_t jack_interrupt(int irq, void *dev_id)
{
	struct sun8iw9_mc_private *ctx = dev_id;
	struct timeval tv;
	u32 tempdata = 0,regval = 0;
	if (ctx->jack_irq_times == RESUME_IRQ || ctx->jack_irq_times == SYSINIT_IRQ){
		is_irq = true;
		ctx->jack_irq_times = OTHER_IRQ;
	}
	pr_debug("SUNXI_HMIC_STS:%x,%d\n",snd_soc_read(ctx->codec, SUNXI_HMIC_STS),__LINE__);
	if ((snd_soc_read(ctx->codec, SUNXI_HMIC_STS)&(1<<JACK_DET_IIN_ST)) && (snd_soc_read(ctx->codec, SUNXI_HMIC_CTRL1)&(1<<JACK_IN_IRQ_EN))) {/*headphone insert*/
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x0<<JACK_OUT_IRQ_EN));
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x0<<JACK_IN_IRQ_EN));
		regval = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
		regval |= 0x1<<JACK_DET_IIN_ST;
		regval &= ~(0x1<<JACK_DET_OUT_ST);
		snd_soc_write(ctx->codec, SUNXI_HMIC_STS, regval);
		/*enable hbias and adc*/
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x1<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x1<<MICADCEN));
		schedule_delayed_work(&ctx->hs_insert_work,msecs_to_jiffies(200));

	} else if((snd_soc_read(ctx->codec, SUNXI_HMIC_STS)&(1<<JACK_DET_OUT_ST)) &&(snd_soc_read(ctx->codec, SUNXI_HMIC_CTRL1)&(1<<JACK_OUT_IRQ_EN))){/*headphone plugout*/
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x0<<JACK_IN_IRQ_EN));
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x0<<JACK_OUT_IRQ_EN));

		regval = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
		regval &= ~(0x1<<JACK_DET_IIN_ST);
		regval |= 0x1<<JACK_DET_OUT_ST;
		snd_soc_write(ctx->codec, SUNXI_HMIC_STS, regval);

		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x0<<MIC_DET_IRQ_EN));

		/*diable hbias and adc*/
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x0<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x0<<MICADCEN));
		snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x1<<JACK_IN_IRQ_EN));
		schedule_delayed_work(&ctx->hs_remove_work,msecs_to_jiffies(1));
	} else if (snd_soc_read(ctx->codec, SUNXI_HMIC_STS)&(1<<MIC_DET_ST)) {/*key*/
		regval = snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
		regval &= ~(0x1<<JACK_DET_IIN_ST);
		regval &= ~(0x1<<JACK_DET_OUT_ST);
		regval |= 0x1<<MIC_DET_ST;
		snd_soc_write(ctx->codec, SUNXI_HMIC_STS, regval);
		do_gettimeofday(&tv);

		if (abs(tv.tv_sec -ctx->tv_headset_plugin.tv_sec) > 2) {
			tempdata =snd_soc_read(ctx->codec, SUNXI_HMIC_STS);
			tempdata = (tempdata&0x1f00)>>8;
			if(tempdata == 2){
				ctx->key_hook = 0;
				ctx->key_voldown = 0;
				ctx->key_volup ++;
				if(ctx->key_volup == 60) {
					pr_debug("Volume + !!\n");
					ctx->key_volup = 0;
					ctx->switch_status |= SND_JACK_BTN_1;
					snd_jack_report(ctx->jack.jack, ctx->switch_status);
					ctx->switch_status &= ~SND_JACK_BTN_1;
					snd_jack_report(ctx->jack.jack, ctx->switch_status);
				}
			} else if (tempdata == 5) {
				ctx->key_volup = 0;
				ctx->key_hook = 0;
				ctx->key_voldown ++;
				if(ctx->key_voldown == 60) {
					pr_debug("Volume - !!\n");
					ctx->key_voldown = 0;
					ctx->switch_status |= SND_JACK_BTN_2;
					snd_jack_report(ctx->jack.jack, ctx->switch_status);
					ctx->switch_status &= ~SND_JACK_BTN_2;
					snd_jack_report(ctx->jack.jack, ctx->switch_status);
				}
			} else if (tempdata == 1 || tempdata == 0) {
				ctx->key_volup = 0;
				ctx->key_voldown = 0;
				ctx->key_hook ++;
				if (ctx->key_hook >= 40) {
					ctx->key_hook = 1;
					if ((ctx->switch_status & SND_JACK_BTN_0) == 0) {
						ctx->switch_status |= SND_JACK_BTN_0;
						snd_jack_report(ctx->jack.jack, ctx->switch_status);
						pr_debug("Hook (1)!!\n");
					}
					schedule_delayed_work(&ctx->hs_button_work,msecs_to_jiffies(180));
				}
			} else {
				pr_debug("tempdata:%x,Key data err:\n",tempdata);
				ctx->key_volup = 0;
				ctx->key_voldown = 0;
				ctx->key_hook = 0;
			}
		} else {
		}
	}
	return IRQ_HANDLED;
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
	ret = snd_soc_dai_set_pll(codec_dai, AIF1_CLK, 0, freq_in, freq_in);
	if (ret < 0) {
		pr_err("err:%s,line:%d\n", __func__, __LINE__);
	}
	/*set system clock source freq */
	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, freq_in, 0);
	if (ret < 0) {
		return ret;
	}
	/*set system clock source freq_in and set the mode as tdm or pcm*/
	ret = snd_soc_dai_set_sysclk(codec_dai, AIF1_CLK, freq_in, 0);
	if (ret < 0) {
		return ret;
	}
	/*set system fmt:api2s:master aif1:slave*/
	ret = snd_soc_dai_set_fmt(cpu_dai, 0);
	if (ret < 0) {
		pr_err("%s, line:%d\n", __func__, __LINE__);
		return ret;
	}

	/*
	* codec: slave. AP: master
	*/
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s, line:%d\n", __func__, __LINE__);
		return ret;
	}

	ret = snd_soc_dai_set_clkdiv(cpu_dai, 0, sample_rate);
	if (ret < 0) {
		pr_err("%s, line:%d\n", __func__, __LINE__);
		return ret;
	}

	return 0;
}

static int ac_speaker_event(struct snd_soc_dapm_widget *w,
				struct snd_kcontrol *k,
				int event)
{
	switch (event) {
		case SND_SOC_DAPM_POST_PMU:
			HS_DBG("[speaker open ]%s,line:%d\n",__func__,__LINE__);
#ifdef AUDIOCODEC_GPIO
			gpio_set_value(item.gpio.gpio, 1);
#endif
			break;
		case SND_SOC_DAPM_PRE_PMD :
			HS_DBG("[speaker close ]%s,line:%d\n",__func__,__LINE__);
#ifdef AUDIOCODEC_GPIO
			gpio_set_value(item.gpio.gpio, 0);
#endif
		default:
			break;
	}
	return 0;
}

static const struct snd_kcontrol_new ac_pin_controls[] = {
	SOC_DAPM_PIN_SWITCH("External Speaker"),
	SOC_DAPM_PIN_SWITCH("Headphone"),
};

static const struct snd_soc_dapm_widget sun8iw9_ac_dapm_widgets[] = {
	SND_SOC_DAPM_SPK("External Speaker", ac_speaker_event),
	SND_SOC_DAPM_MIC("External MainMic", NULL),
	SND_SOC_DAPM_MIC("HeadphoneMic", NULL),
};

static const struct snd_soc_dapm_route audio_map[] = {
	{"External Speaker", NULL, "SPK1P"},
	{"External Speaker", NULL, "SPK1N"},

	{"MainMic Bias", NULL, "External MainMic"},
	{"MIC1P", NULL, "MainMic Bias"},
	{"MIC1N", NULL, "MainMic Bias"},

	{"MIC2", NULL, "HeadphoneMic"},
};
/*
 * Card initialization
 */
static int sunxi_daudio_init(struct snd_soc_pcm_runtime *runtime)
{
	int ret;
	struct snd_soc_codec *codec = runtime->codec;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct sun8iw9_mc_private *ctx = snd_soc_card_get_drvdata(runtime->card);

	ctx->codec = runtime->codec;
	ret = snd_soc_jack_new(ctx->codec, "sunxi Audio Jack",
			       SND_JACK_HEADSET | SND_JACK_HEADPHONE | SND_JACK_BTN_0 | SND_JACK_BTN_1 | SND_JACK_BTN_2,
			       &ctx->jack);
	if (ret) {
		pr_err("jack creation failed\n");
		return ret;
	}

	snd_jack_set_key(ctx->jack.jack, SND_JACK_BTN_0, KEY_MEDIA);
	snd_jack_set_key(ctx->jack.jack, SND_JACK_BTN_1, KEY_VOLUMEUP);
	snd_jack_set_key(ctx->jack.jack, SND_JACK_BTN_2, KEY_VOLUMEDOWN);

	snd_soc_dapm_disable_pin(&codec->dapm,	"HPOUTR");
	snd_soc_dapm_disable_pin(&codec->dapm,	"HPOUTL");
	snd_soc_dapm_disable_pin(&codec->dapm,	"EAROUTP");
	snd_soc_dapm_disable_pin(&codec->dapm,	"EAROUTN");
	snd_soc_dapm_disable_pin(&codec->dapm,	"SPKL");
	snd_soc_dapm_disable_pin(&codec->dapm,	"SPKR");

	snd_soc_dapm_disable_pin(&runtime->card->dapm,	"External Speaker");
	snd_soc_dapm_disable_pin(&runtime->card->dapm,	"Headphone");
	snd_soc_dapm_disable_pin(&runtime->card->dapm,	"Earpiece");
	snd_soc_dapm_sync(dapm);

	return 0;
}

static int sunxi_suspend(struct snd_soc_card *card)
{
	struct sun8iw9_mc_private *ctx = snd_soc_card_get_drvdata(card);

	disable_irq(ctx->jackirq);
	pr_debug("[codec-machine]  suspend.\n");

	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<MIC_DET_IRQ_EN), (0x0<<MIC_DET_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x0<<JACK_IN_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x0<<JACK_OUT_IRQ_EN));
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<JACKDETEN), (0x0<<JACKDETEN));

	return 0;
}

static int sunxi_resume(struct snd_soc_card *card)
{
	struct sun8iw9_mc_private *ctx = snd_soc_card_get_drvdata(card);
	enable_irq(ctx->jackirq);
	ctx->jack_irq_times = RESUME_IRQ;
	/*0x314*/
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0xffff<<0), (0x0<<0));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0x1f<<8), (0x17<<8));/*0x318*/
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_STS, (0xffff<<0), (0x0<<0));	/*0x1c*/
	snd_soc_update_bits(ctx->codec, MDET_CTRL, (0xffff<<0), (0x0<<0));
	if (ctx->hp_detect_case == HP_DETECT_LOW)
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<AUTOPLEN), (0x1<<AUTOPLEN));
	else {
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<AUTOPLEN), (0x0<<AUTOPLEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<INNERRESEN), (0x1<<INNERRESEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x1<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x1<<MICADCEN));
	}
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<JACKDETEN), (0x1<<JACKDETEN));

	schedule_delayed_work(&ctx->hs_init_work,msecs_to_jiffies(280));

	/*enable jack in /out irq*//*0x310*/
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<INNERRESEN), (0x1<<INNERRESEN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x1<<JACK_IN_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x1<<JACK_OUT_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0xf<<HMIC_N), (0xf<<HMIC_N));

	pr_info("[codec-machine]  resume.\n");
	return 0;
}

static struct snd_soc_ops sunxi_sndpcm_ops = {
       .hw_params              = sunxi_sndpcm_hw_params,
};
#endif

static int goni_voice_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	if (params_rate(params) != 8000)
		return -EINVAL;

	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_LEFT_J |
			SND_SOC_DAIFMT_IB_IF | SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0)
		return ret;

	return 0;
}

static struct snd_soc_ops goni_voice_ops = {
	.hw_params = goni_voice_hw_params,
};

static struct snd_soc_dai_link sunxi_sndpcm_dai_link[] = {
	{
	.name 			= "audiocodec",
	.stream_name 	= "SUNXI-CODEC",
	.cpu_dai_name 	= "sunxi-codec",
	.codec_dai_name = "codec-aif1",
	.platform_name 	= "sunxi-pcm-codec-audio",
	.codec_name 	= "sunxi-pcm-codec",
	.init 			= sunxi_daudio_init,
#ifdef CONFIG_ARCH_SUN8IW9
    .ops = &sunxi_sndpcm_ops,
#endif
	},{
	.name = "sun8iw9 Voice",
	.stream_name = "Voice",
	.cpu_dai_name = "sunxi-bbcodec",
	.codec_dai_name = "codec-aif2",
	.codec_name = "sunxi-pcm-codec",
	.ops = &goni_voice_ops,
	},
};

static struct snd_soc_card snd_soc_sunxi_sndpcm = {
	.name 		= "audiocodec",
	.owner 		= THIS_MODULE,
	.dai_link 	= sunxi_sndpcm_dai_link,
	.num_links 	= ARRAY_SIZE(sunxi_sndpcm_dai_link),
	.dapm_widgets = sun8iw9_ac_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(sun8iw9_ac_dapm_widgets),
	.dapm_routes = audio_map,
	.num_dapm_routes = ARRAY_SIZE(audio_map),
	.controls = ac_pin_controls,
	.num_controls = ARRAY_SIZE(ac_pin_controls),
	.suspend_post = sunxi_suspend,
	.resume_post = sunxi_resume,
};

static int sun8iw9_machine_probe(struct platform_device *pdev)
{
	int ret = 0;
	script_item_u val;
	struct sun8iw9_mc_private *ctx;
	script_item_value_type_e  type;

	HS_DBG("Entry %s\n", __func__);

	/*get the default pa val(close)*/
	type = script_get_item("audio0", "audio_pa_ctrl", &item);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		pr_err("script_get_item return type err\n");
		return -EFAULT;
	}
	/*request pa gpio*/
	ret = gpio_request(item.gpio.gpio, NULL);
	if (0 != ret) {
		/*for homlet case, it doesn't need pa gpio.so run program continue*/
		pr_err("request gpio failed!\n");
	} else {
		/*
		* config gpio info of audio_pa_ctrl, the default pa config is close(check pa sys_config1.fex)
		*/
		gpio_direction_output(item.gpio.gpio, 1);
		gpio_set_value(item.gpio.gpio, 0);
	}

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_ATOMIC);
	if (!ctx) {
		pr_err("allocation failed\n");
		return -ENOMEM;
	}
	ctx->jackirq = platform_get_irq(pdev, 0);
	if (ctx->jackirq < 0) {
		pr_err("[audio]irq failed\n");
		ret = -ENODEV;
		goto err1;
	}
pr_debug("%s,l:%d,ctx->jackirq:%d\n", __func__, __LINE__, ctx->jackirq);
	type = script_get_item("audio0", "hp_detect_case", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] type err!\n");
    }
	ctx->hp_detect_case = val.val;
	if (ctx->hp_detect_case == HP_DETECT_LOW) {
		JACK_IN_IRQ_EN = 4;
		JACK_OUT_IRQ_EN = 3;
		JACK_DET_OUT_ST = 4;
		JACK_DET_IIN_ST = 3;
	} else {
		JACK_IN_IRQ_EN = 3;
		JACK_OUT_IRQ_EN = 4;
		JACK_DET_OUT_ST = 3;
		JACK_DET_IIN_ST = 4;
	}
	pr_debug("%s,line:%d,JACK_IN_IRQ_EN:%d,JACK_OUT_IRQ_EN:%d,JACK_DET_OUT_ST:%d,JACK_DET_IIN_ST:%d\n",\
	__func__,__LINE__,JACK_IN_IRQ_EN, JACK_OUT_IRQ_EN, JACK_DET_OUT_ST, JACK_DET_IIN_ST);

	/* register the soc card */
	snd_soc_sunxi_sndpcm.dev = &pdev->dev;
	snd_soc_card_set_drvdata(&snd_soc_sunxi_sndpcm, ctx);
	ret = snd_soc_register_card(&snd_soc_sunxi_sndpcm);
	if (ret) {
		pr_err("snd_soc_register_card failed %d\n", ret);
		return ret;
	}
	platform_set_drvdata(pdev, &snd_soc_sunxi_sndpcm);

	if (ctx->hp_detect_case == HP_DETECT_HIGH)
	{/*yj*/
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<INNERRESEN), (0x1<<INNERRESEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<HMICBIASEN), (0x1<<HMICBIASEN));
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<MICADCEN), (0x1<<MICADCEN));
	}
	ctx->jack_irq_times = SYSINIT_IRQ;
	/*
	*initial the parameters for judge switch state
	*/
	ctx->HEADSET_DATA = 0x05;
	INIT_DELAYED_WORK(&ctx->hs_insert_work, sunxi_check_hs_insert_status);
	INIT_DELAYED_WORK(&ctx->hs_button_work, sunxi_check_hs_button_status);
	INIT_DELAYED_WORK(&ctx->hs_remove_work, sunxi_check_hs_remove_status);
	INIT_DELAYED_WORK(&ctx->hs_init_work, sunxi_hs_init_work);
	mutex_init(&ctx->jack_mlock);

	pr_debug("register jack interrupt.,jackirq:%d\n",ctx->jackirq);
	ret = request_irq(ctx->jackirq, jack_interrupt, 0, "audio jack irq", ctx);

	/*0x314*/
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0xffff<<0), (0x0<<0));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL2, (0x1f<<8), (0x17<<8));
	/*0x318*/
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_STS, (0xffff<<0), (0x0<<0));
	/*0x1c*/
	snd_soc_update_bits(ctx->codec, MDET_CTRL, (0xffff<<0), (0x0<<0));

	if (ctx->hp_detect_case == HP_DETECT_LOW)
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<AUTOPLEN), (0x1<<AUTOPLEN));
	else {
		snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<AUTOPLEN), (0x0<<AUTOPLEN));
	}
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<JACKDETEN), (0x1<<JACKDETEN));

	schedule_delayed_work(&ctx->hs_init_work,msecs_to_jiffies(280));

	/*enable jack in /out irq*//*0x310*/
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x1<<JACK_IN_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x1<<JACK_OUT_IRQ_EN));
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<INNERRESEN), (0x1<<INNERRESEN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0xf<<HMIC_N), (0xf<<HMIC_N));

	pr_debug("%s,line:%d,0x310:%x,0x314:%x,0x318:%x,0x1C:%x,0x1D:%x\n",\
	__func__,__LINE__, snd_soc_read(ctx->codec, 0x310), snd_soc_read(ctx->codec, 0x314),\
	snd_soc_read(ctx->codec, 0x318), snd_soc_read(ctx->codec, 0x1C), snd_soc_read(ctx->codec, 0x1D));

	return 0;
err1:
	return ret;
}

static void snd_sun8iw9_unregister_jack(struct sun8iw9_mc_private *ctx)
{
	/* Set process button events to false so that the button
	   delayed work will not be scheduled.*/
	ctx->process_button_events = false;

	cancel_delayed_work_sync(&ctx->hs_insert_work);
	cancel_delayed_work_sync(&ctx->hs_button_work);
	cancel_delayed_work_sync(&ctx->hs_remove_work);
	cancel_delayed_work_sync(&ctx->hs_init_work);
}

static void sun8iw9_machine_shutdown(struct platform_device *pdev)
{
	struct snd_soc_card *soc_card = platform_get_drvdata(pdev);
	struct sun8iw9_mc_private *ctx = snd_soc_card_get_drvdata(soc_card);

	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_IN_IRQ_EN), (0x0<<JACK_IN_IRQ_EN));
	snd_soc_update_bits(ctx->codec, SUNXI_HMIC_CTRL1, (0x1<<JACK_OUT_IRQ_EN), (0x0<<JACK_OUT_IRQ_EN));
	snd_soc_update_bits(ctx->codec, JACK_MIC_CTRL, (0x1<<JACKDETEN), (0x0<<JACKDETEN));
	snd_sun8iw9_unregister_jack(ctx);
}

static struct resource sun8iw9_audio_res[] = {
	[0] = {
		.start	= SUNXI_IRQ_AC_DET,
		.end	= SUNXI_IRQ_AC_DET,
		.flags	= IORESOURCE_IRQ,
	},
};

/*data relating*/
static struct platform_device sun8iw9_machine_device = {
	.name = "sun8iw9_machine",
	.id = -1,
	.num_resources	= ARRAY_SIZE(sun8iw9_audio_res),
	.resource	= sun8iw9_audio_res,
};

/*method relating*/
static struct platform_driver sun8iw9_machine_driver = {
	.driver = {
		.name = "sun8iw9_machine",
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
	},
	.probe = sun8iw9_machine_probe,
	.shutdown = sun8iw9_machine_shutdown,
};

static int __init sun8iw9_machine_init(void)
{
	int err = 0;
	script_item_u val;
	script_item_value_type_e  type;

	type = script_get_item("audio0", "audio_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] audio_used type err!\n");
	} else {
		audio_used = val.val;
	}

	if (audio_used) {
		if((err = platform_device_register(&sun8iw9_machine_device)) < 0)
			return err;

		if ((err = platform_driver_register(&sun8iw9_machine_driver)) < 0)
			return err;
	}
	return 0;
}
module_init(sun8iw9_machine_init);

static void __exit sun8iw9_machine_exit(void)
{
	platform_driver_unregister(&sun8iw9_machine_driver);
}
module_exit(sun8iw9_machine_exit);
/*
*	/sys/module/sun8iw9_machine/parameters/switch_state
*/
module_param_named(switch_state, switch_state, int, S_IRUGO | S_IWUSR);

MODULE_AUTHOR("huangxin");
MODULE_DESCRIPTION("SUNXI_sndpcm ALSA SoC audio driver");
MODULE_LICENSE("GPL");
