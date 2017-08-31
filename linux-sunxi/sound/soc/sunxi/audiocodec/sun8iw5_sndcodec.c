  /*
   * sound\soc\sunxi\sunxi_sun50iw1codec.c
   * (C) Copyright 2014-2017
   * Reuuimlla Technology Co., Ltd. <www.allwinnertech.com>
   * YoungGuo <guoyingyang@allwinnertech.com>
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
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/regulator/consumer.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/jack.h>
#include <sound/tlv.h>
#include <mach/sys_config.h>
#include <mach/gpio.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm.h>
#include <linux/power/scenelock.h>
#include <linux/pinctrl/consumer.h>

#include "sunxi_codecdma.h"
#include "sun8iw5_sndcodec.h"

/* #define AIF1_FPGA_LOOPBACK_TEST */
#define codec_RATES  (SNDRV_PCM_RATE_8000_192000|SNDRV_PCM_RATE_KNOT)
#define codec_FORMATS (SNDRV_PCM_FMTBIT_S8 | SNDRV_PCM_FMTBIT_S16_LE | \
				SNDRV_PCM_FMTBIT_S18_3LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE)

static bool src_function_en;

static struct label reg_labels[] = {
	LABEL(SUNXI_DA_CTL),
	LABEL(SUNXI_DA_FAT0),
	LABEL(SUNXI_DA_FAT1),
	LABEL(SUNXI_DA_TXFIFO),
	LABEL(SUNXI_DA_RXFIFO),
	LABEL(SUNXI_DA_FCTL),
	LABEL(SUNXI_DA_FSTA),
	LABEL(SUNXI_DA_INT),
	LABEL(SUNXI_DA_ISTA),
	LABEL(SUNXI_DA_CLKD),
	LABEL(SUNXI_DA_TXCNT),
	LABEL(SUNXI_DA_RXCNT),
	LABEL(SUNXI_DA_TXCHSEL),
	LABEL(SUNXI_DA_TXCHMAP),
	LABEL(SUNXI_DA_RXCHSEL),
	LABEL(SUNXI_DA_RXCHMAP),
	LABEL(SUNXI_CHIP_AUDIO_RST),
	LABEL(SUNXI_SYSCLK_CTL),
	LABEL(SUNXI_MOD_CLK_ENA),
	LABEL(SUNXI_MOD_RST_CTL),
	LABEL(SUNXI_SYS_SR_CTRL),
	LABEL(SUNXI_SYS_SRC_CLK),
	LABEL(SUNXI_AIF1_CLK_CTRL),
	LABEL(SUNXI_AIF1_ADCDAT_CTRL),
	LABEL(SUNXI_AIF1_DACDAT_CTRL),
	LABEL(SUNXI_AIF1_MXR_SRC),
	LABEL(SUNXI_AIF1_VOL_CTRL1),
	LABEL(SUNXI_AIF1_VOL_CTRL2),
	LABEL(SUNXI_AIF1_VOL_CTRL3),
	LABEL(SUNXI_AIF1_VOL_CTRL4),
	LABEL(SUNXI_AIF1_MXR_GAIN),
	LABEL(SUNXI_AIF1_RXD_CTRL),
	LABEL(SUNXI_AIF2_CLK_CTRL),
	LABEL(SUNXI_AIF2_ADCDAT_CTRL),
	LABEL(SUNXI_AIF2_DACDAT_CTRL),
	LABEL(SUNXI_AIF2_MXR_SRC),
	LABEL(SUNXI_AIF2_VOL_CTRL1),
	LABEL(SUNXI_AIF2_VOL_CTRL2),
	LABEL(SUNXI_AIF2_MXR_GAIN),
	LABEL(SUNXI_AIF2_RXD_CTRL),
	LABEL(SUNXI_AIF3_CLK_CTRL),
	LABEL(SUNXI_AIF3_ADCDAT_CTRL),
	LABEL(SUNXI_AIF3_DACDAT_CTRL),
	LABEL(SUNXI_AIF3_SGP_CTRL),
	LABEL(SUNXI_AIF3_RXD_CTRL),
	LABEL(SUNXI_ADC_DIG_CTRL),
	LABEL(SUNXI_ADC_VOL_CTRL),
	LABEL(SUNXI_ADC_DBG_CTRL),
	LABEL(SUNXI_HMIC_CTRL1),
	LABEL(SUNXI_HMIC_CTRL2),
	LABEL(SUNXI_HMIC_STS),
	LABEL(SUNXI_DAC_DIG_CTRL),
	LABEL(SUNXI_DAC_VOL_CTRL),
	LABEL(SUNXI_DAC_DBG_CTRL),
	LABEL(SUNXI_DAC_MXR_SRC),
	LABEL(SUNXI_DAC_MXR_GAIN),
	LABEL(SUNXI_AC_DAPHHPFC),
	LABEL(SUNXI_AC_DAPLHPFC),
	LABEL(SUNXI_AC_DAPOPT),
	LABEL(SUNXI_AGC_ENA),
	LABEL(SUNXI_DRC_ENA),
	LABEL(SUNXI_SRC_BISTCR),
	LABEL(SUNXI_SRC_BISTST),
	LABEL(SUNXI_SRC1_CTRL1),
	LABEL(SUNXI_SRC1_CTROL2),
	LABEL(SUNXI_SRC1_CTRL3),
	LABEL(SUNXI_SRC1_CTRL4),
	LABEL(SUNXI_SRC2_CTRL1),
	LABEL(SUNXI_SRC2_CTRL2),
	LABEL(SUNXI_SRC2_CTRL3),
	LABEL(SUNXI_SRC2_CTRL4),
	LABEL(HP_VOLC),
	LABEL(LOMIXSC),
	LABEL(ROMIXSC),
	LABEL(DAC_PA_SRC),
	LABEL(PHONEIN_GCTRL),
	LABEL(LINEIN_GCTRL),
	LABEL(MICIN_GCTRL),
	LABEL(PAEN_HP_CTRL),
	LABEL(PHONEOUT_CTRL),
	LABEL(LINEOUT_VOLC),
	LABEL(MIC2G_LINEEN_CTRL),
	LABEL(MIC1G_MICBIAS_CTRL),
	LABEL(LADCMIXSC),
	LABEL(RADCMIXSC),
	LABEL(ADC_AP_EN),
	LABEL(ADDA_APT0),
	LABEL(ADDA_APT1),
	LABEL(ADDA_APT2),
	LABEL(BIAS_AD16_CAL_CTRL),
	LABEL(BIAS_DA16_CAL_CTRL),
	LABEL(DA16CALI),
	LABEL(DA16VERIFY),
	LABEL(BIASCALI),
	LABEL(BIASVERIFY),
	LABEL_END
};

struct voltage_supply {
	struct regulator *hp_ldo;
};
struct gain_config {
	u32 headphonevol;
	u32 maingain;
	u32 headsetmicgain;
	u32 dac_digital_vol;
};

struct sunxi_codec_priv {

	struct mutex dac_mutex;
	struct mutex adc_mutex;
	struct mutex aifclk_mutex;
	struct snd_soc_codec *codec;
	struct gain_config gain_config;
	struct voltage_supply vol_supply;
	struct clk *srcclk;
	script_item_u audio_pa_ctrl;
	script_item_u audio_pa_en;
	u32 pa_gpio_reverse;
	u32 aif2_used;
	u32 aif3_used;
	u32 dac_enable;
	u32 adc_enable;
	u32 aif1_clken;
	u32 aif2_clken;
	u32 aif3_clken;
	u32 pa_sleep_time;
	u32 aif1_lrlk_div;
	u32 aif2_lrlk_div;
	u32 headphone_direct_used;

};

static unsigned int read_prcm_wvalue(unsigned int addr)
{
	unsigned int reg;
	reg = readl(ADDA_PR_CFG_REG);
	reg |= (0x1 << 28);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= ~(0x1 << 24);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= ~(0x1f << 16);
	reg |= (addr << 16);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= (0xff << 0);

	return reg;
}

static void write_prcm_wvalue(unsigned int addr, unsigned int val)
{
	unsigned int reg;
	reg = readl(ADDA_PR_CFG_REG);
	reg |= (0x1 << 28);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= ~(0x1f << 16);
	reg |= (addr << 16);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= ~(0xff << 8);
	reg |= (val << 8);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg |= (0x1 << 24);
	writel(reg, ADDA_PR_CFG_REG);

	reg = readl(ADDA_PR_CFG_REG);
	reg &= ~(0x1 << 24);
	writel(reg, ADDA_PR_CFG_REG);
}
#if 0
/**
* codec_wrreg_bits - update codec register bits
* @reg: codec register
* @mask: register mask
* @value: new value
*
* Writes new register value.
* Return 1 for change else 0.
*/
static int codec_wrreg_prcm_bits(unsigned short reg, unsigned int mask,
				 unsigned int value)
{
	unsigned int old, new;

	old = read_prcm_wvalue(reg);
	new = (old & ~mask) | value;
	write_prcm_wvalue(reg, new);

	return 0;
}

static int codec_wr_prcm_control(u32 reg, u32 mask, u32 shift, u32 val)
{
	u32 reg_val;
	reg_val = val << shift;
	mask = mask << shift;
	codec_wrreg_prcm_bits(reg, mask, reg_val);
	return 0;
}

#endif
/**
* codec_wrreg_bits - update codec register bits
* @reg: codec register
* @mask: register mask
* @value: new value
*
* Writes new register value.
* Return 1 for change else 0.
*/
int codec_wrreg_bits(unsigned short reg, unsigned int mask, unsigned int value)
{
	unsigned int old, new;

	old = codec_rdreg(reg);
	new = (old & ~mask) | value;
	codec_wrreg(reg, new);

	return 0;
}

int codec_wr_control(u32 reg, u32 mask, u32 shift, u32 val)
{
	u32 reg_val;
	reg_val = val << shift;
	mask = mask << shift;
	codec_wrreg_bits(reg, mask, reg_val);
	return 0;
}

void aif_pin_request(__u32 aif_index, bool enable)
{
	script_item_u *pin_list;
	int pin_count = 0;
	int pin_index;

	/* get pin sys_config info */
	if (aif_index == 2)
		pin_count = script_get_pio_list("codec_aif2", &pin_list);
	else if (aif_index == 3)
		pin_count = script_get_pio_list("codec_aif3", &pin_list);

	if (pin_count == 0) {
		printk("pin count 0\n");
		return;
	}
	/* request pin individually */
	for (pin_index = 0; pin_index < pin_count; pin_index++) {
		struct gpio_config *pin_cfg = &(pin_list[pin_index].gpio);
		char pin_name[SUNXI_PIN_NAME_MAX_LEN];
		unsigned long config;
		/* valid pin of sunxi-pinctrl,
		 * config pin attributes individually.
		 */
		sunxi_gpio_to_name(pin_cfg->gpio, pin_name);
		if (enable)
			config =
			    SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC,
					      pin_cfg->mul_sel);
		else
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
	}

	return;
}

static ssize_t show_audio_reg(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	int count = 0;
	int i = 0;
	int reg_group = 0;
	printk("%s,line:%d\n", __func__, __LINE__);

	count += sprintf(buf, "dump audio reg:\n");

	while (reg_labels[i].name != NULL) {
		if (reg_labels[i].value == 0) {
			reg_group++;
		}
		if (reg_group == 1) {
			count +=
			    sprintf(buf + count, "%s 0x%p: 0x%x\n",
				    reg_labels[i].name,
				    (baseaddr + reg_labels[i].value),
				    readl(baseaddr + reg_labels[i].value));
		} else if (reg_group == 2) {
			count +=
			    sprintf(buf + count, "%s 0x%x: 0x%x\n",
				    reg_labels[i].name, (reg_labels[i].value),
				    read_prcm_wvalue(reg_labels[i].value));
		}
		i++;
	}

	return count;
}

/* ex:
	read:
		echo 0,1,0x00> audio_reg
		echo 0,2,0x00> audio_reg
	write:
		echo 1,1,0x00,0xa > audio_reg
		echo 1,2,0x00,0xff > audio_reg
*/
static ssize_t store_audio_reg(struct device *dev,
			       struct device_attribute *attr, const char *buf,
			       size_t count)
{
	int ret;
	int input_reg_group = 0;
	int input_reg_offset = 0;
	int input_reg_val = 0;
	int reg_val_read;
	int rw_flag;

	printk("%s,line:%d\n", __func__, __LINE__);
	ret =
	    sscanf(buf, "%d,%d,0x%x,0x%x", &rw_flag, &input_reg_group,
		   &input_reg_offset, &input_reg_val);
	printk("ret:%d, reg_group:%d, reg_offset:%d, reg_val:0x%x\n", ret,
	       input_reg_group, input_reg_offset, input_reg_val);

	if (!(input_reg_group == 1 || input_reg_group == 2)) {
		printk("not exist reg group\n");
		ret = count;
		goto out;
	}
	if (!(rw_flag == 1 || rw_flag == 0)) {
		printk("not rw_flag\n");
		ret = count;
		goto out;
	}
	if (input_reg_group == 1) {
		if (rw_flag) {
			writel(input_reg_val, baseaddr + input_reg_offset);
		} else {
			reg_val_read = readl(baseaddr + input_reg_offset);
			printk("\n\n Reg[0x%x] : 0x%x\n\n", input_reg_offset,
			       reg_val_read);
		}
	} else if (input_reg_group == 2) {
		if (rw_flag) {
			write_prcm_wvalue(input_reg_offset,
					  input_reg_val & 0xff);
		} else {
			reg_val_read = read_prcm_wvalue(input_reg_offset);
			printk("\n\n Reg[0x%x] : 0x%x\n\n", input_reg_offset,
			       reg_val_read);
		}
	}

	ret = count;

      out:
	return ret;
}

static DEVICE_ATTR(audio_reg, 0644, show_audio_reg, store_audio_reg);

static struct attribute *audio_debug_attrs[] = {
	&dev_attr_audio_reg.attr,
	NULL,
};

static struct attribute_group audio_debug_attr_group = {
	.name = "audio_reg_debug",
	.attrs = audio_debug_attrs,
};

static void codec_init(struct sunxi_codec_priv *sunxi_internal_codec)
{
	if (sunxi_internal_codec->headphone_direct_used) {
		snd_soc_update_bits(sunxi_internal_codec->codec, PAEN_HP_CTRL,
				    (0x3 << HPCOM_FC), 0x3);
		snd_soc_update_bits(sunxi_internal_codec->codec, PAEN_HP_CTRL,
				    (0x1 << COMPTEN), 0x1);
	} else {
		snd_soc_update_bits(sunxi_internal_codec->codec, PAEN_HP_CTRL,
				    (0x3 << HPCOM_FC), 0x0);
		snd_soc_update_bits(sunxi_internal_codec->codec, PAEN_HP_CTRL,
				    (0x1 << COMPTEN), 0x0);
	}

	if (sunxi_internal_codec->aif2_used)
		aif_pin_request(2, 1);
	if (sunxi_internal_codec->aif3_used)
		aif_pin_request(3, 1);

	if (gpio_is_valid(sunxi_internal_codec->audio_pa_en.gpio.gpio)) {
		gpio_direction_output(sunxi_internal_codec->audio_pa_en.gpio.
				      gpio, 1);
		gpio_set_value(sunxi_internal_codec->audio_pa_en.gpio.gpio, 1);
	}

	if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
		gpio_direction_output(sunxi_internal_codec->audio_pa_ctrl.gpio.
				      gpio, 1);
		/* mute pa */
		if (sunxi_internal_codec->pa_gpio_reverse) {
			gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.
				       gpio, 1);
		} else {
			gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.
				       gpio, 0);
		}
	}
	snd_soc_update_bits(sunxi_internal_codec->codec, HP_VOLC,
			    (0x3f << HPVOL),
			    (sunxi_internal_codec->gain_config.
			     headphonevol << HPVOL));
	snd_soc_update_bits(sunxi_internal_codec->codec, MIC1G_MICBIAS_CTRL,
			    (0x7 << MIC1BOOST),
			    (sunxi_internal_codec->gain_config.
			     maingain << MIC1BOOST));
	snd_soc_update_bits(sunxi_internal_codec->codec, MIC2G_LINEEN_CTRL,
			    (0x7 << MIC2BOOST),
			    (sunxi_internal_codec->gain_config.
			     headsetmicgain << MIC2BOOST));

	snd_soc_update_bits(sunxi_internal_codec->codec, DAC_PA_SRC,
			    (0x1 << DACALEN), (1 << DACALEN));
	snd_soc_update_bits(sunxi_internal_codec->codec, DAC_PA_SRC,
			    (0x1 << DACAREN), (1 << DACAREN));

	snd_soc_update_bits(sunxi_internal_codec->codec, PAEN_HP_CTRL,
			    (0x1 << HPPAEN), (0x1 << HPPAEN));

}

static int late_enable_dac(struct snd_soc_dapm_widget *w,
			   struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);
	mutex_lock(&sunxi_codec->dac_mutex);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		pr_debug("%s,line:%d\n", __func__, __LINE__);
		if (sunxi_codec->dac_enable == 0) {
			/*enable dac module clk */
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << DAC_DIGITAL_MOD_CLK_EN),
					    (0x1 << DAC_DIGITAL_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << DAC_DIGITAL_MOD_RST_CTL),
					    (0x1 << DAC_DIGITAL_MOD_RST_CTL));
			snd_soc_update_bits(codec, SUNXI_DAC_DIG_CTRL,
					    (0x1 << ENDA), (0x1 << ENDA));
		}
		sunxi_codec->dac_enable++;
		break;
	case SND_SOC_DAPM_POST_PMD:
		if (sunxi_codec->dac_enable > 0) {
			sunxi_codec->dac_enable--;
			if (sunxi_codec->dac_enable == 0) {
				snd_soc_update_bits(codec, SUNXI_DAC_DIG_CTRL,
						    (0x1 << ENDA),
						    (0x0 << ENDA));
				/*disable dac module clk */
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 <<
						     DAC_DIGITAL_MOD_CLK_EN),
						    (0x0 <<
						     DAC_DIGITAL_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 <<
						     DAC_DIGITAL_MOD_RST_CTL),
						    (0x0 <<
						     DAC_DIGITAL_MOD_RST_CTL));
			}
		}
		break;
	}
	mutex_unlock(&sunxi_codec->dac_mutex);
	return 0;
}

static int late_enable_adc(struct snd_soc_dapm_widget *w,
			   struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);
	mutex_lock(&sunxi_codec->adc_mutex);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		if (sunxi_codec->adc_enable == 0) {
			/*enable adc module clk */
			pr_debug("%s,line:%d\n", __func__, __LINE__);
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << ADC_DIGITAL_MOD_CLK_EN),
					    (0x1 << ADC_DIGITAL_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << ADC_DIGITAL_MOD_RST_CTL),
					    (0x1 << ADC_DIGITAL_MOD_RST_CTL));
			snd_soc_update_bits(codec, SUNXI_ADC_DIG_CTRL,
					    (0x1 << ENAD), (0x1 << ENAD));
		}
		sunxi_codec->adc_enable++;
		break;
	case SND_SOC_DAPM_POST_PMD:
		if (sunxi_codec->adc_enable > 0) {
			sunxi_codec->adc_enable--;
			if (sunxi_codec->adc_enable == 0) {
				snd_soc_update_bits(codec, SUNXI_ADC_DIG_CTRL,
						    (0x1 << ENAD),
						    (0x0 << ENAD));
				/*disable adc module clk */
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 <<
						     ADC_DIGITAL_MOD_CLK_EN),
						    (0x0 <<
						     ADC_DIGITAL_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 <<
						     ADC_DIGITAL_MOD_RST_CTL),
						    (0x0 <<
						     ADC_DIGITAL_MOD_RST_CTL));
			}
		}
		break;
	}
	mutex_unlock(&sunxi_codec->adc_mutex);
	return 0;
}

static int ac_headphone_event(struct snd_soc_dapm_widget *w,
			      struct snd_kcontrol *k, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_internal_codec =
	    snd_soc_codec_get_drvdata(codec);
	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/*open */
		pr_debug("post:open:%s,line:%d\n", __func__, __LINE__);
		snd_soc_update_bits(codec, DAC_PA_SRC, (0x1 << LHPPAMUTE),
				    (0x1 << LHPPAMUTE));
		snd_soc_update_bits(codec, DAC_PA_SRC, (0x1 << RHPPAMUTE),
				    (0x1 << RHPPAMUTE));
		usleep_range(2000, 3000);
		/* enable pa */
		if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
			if (sunxi_internal_codec->pa_gpio_reverse) {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 0);
			} else {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 1);
			}
		}
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/*close */
		pr_debug("pre:close:%s,line:%d\n", __func__, __LINE__);
		if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
			if (sunxi_internal_codec->pa_gpio_reverse) {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 1);
			} else {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 0);
			}
		}
		snd_soc_update_bits(codec, DAC_PA_SRC, (0x1 << LHPPAMUTE),
				    (0x0 << LHPPAMUTE));
		snd_soc_update_bits(codec, DAC_PA_SRC, (0x1 << RHPPAMUTE),
				    (0x0 << RHPPAMUTE));
		break;
	}
	return 0;
}

int ac_aif1clk(struct snd_soc_dapm_widget *w,
	       struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);

	mutex_lock(&sunxi_codec->aifclk_mutex);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		if (sunxi_codec->aif1_clken == 0) {
			/*enable AIF1CLK */
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x1 << AIF1CLK_ENA),
					    (0x1 << AIF1CLK_ENA));
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << AIF1_MOD_CLK_EN),
					    (0x1 << AIF1_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << AIF1_MOD_RST_CTL),
					    (0x1 << AIF1_MOD_RST_CTL));

			/*enable systemclk */
			if (sunxi_codec->aif2_clken == 0
			    && sunxi_codec->aif3_clken == 0)
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << SYSCLK_ENA),
						    (0x1 << SYSCLK_ENA));
		}
		sunxi_codec->aif1_clken++;
		break;
	case SND_SOC_DAPM_POST_PMD:
		if (sunxi_codec->aif1_clken > 0) {
			sunxi_codec->aif1_clken--;
			if (sunxi_codec->aif1_clken == 0) {
				/*disable AIF1CLK */
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << AIF1CLK_ENA),
						    (0x0 << AIF1CLK_ENA));
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 << AIF1_MOD_CLK_EN),
						    (0x0 << AIF1_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 << AIF1_MOD_RST_CTL),
						    (0x0 << AIF1_MOD_RST_CTL));
				/*DISABLE systemclk */
				if (sunxi_codec->aif2_clken == 0
				    && sunxi_codec->aif3_clken == 0)
					snd_soc_update_bits(codec,
							    SUNXI_SYSCLK_CTL,
							    (0x1 << SYSCLK_ENA),
							    (0x0 <<
							     SYSCLK_ENA));
			}
		}
		break;
	}
	mutex_unlock(&sunxi_codec->aifclk_mutex);
	return 0;
}

int ac_aif2clk(struct snd_soc_dapm_widget *w,
	       struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);

	mutex_lock(&sunxi_codec->aifclk_mutex);
	pr_debug("%s,%d \n", __func__, __LINE__);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		if (sunxi_codec->aif2_clken == 0) {
			/*enable AIF2CLK */
			pr_debug(" enable %s,%d \n", __func__, __LINE__);
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x1 << AIF2CLK_ENA),
					    (0x1 << AIF2CLK_ENA));
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << AIF2_MOD_CLK_EN),
					    (0x1 << AIF2_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << AIF2_MOD_RST_CTL),
					    (0x1 << AIF2_MOD_RST_CTL));
			/*enable systemclk */
			if (sunxi_codec->aif1_clken == 0
			    && sunxi_codec->aif3_clken == 0)
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << SYSCLK_ENA),
						    (0x1 << SYSCLK_ENA));
		}
		sunxi_codec->aif2_clken++;
		break;
	case SND_SOC_DAPM_POST_PMD:
		if (sunxi_codec->aif2_clken > 0) {
			sunxi_codec->aif2_clken--;
			if (sunxi_codec->aif2_clken == 0) {
				/*disable AIF2CLK */
				pr_debug("disable,%s,%d \n", __func__, __LINE__);
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << AIF2CLK_ENA),
						    (0x0 << AIF2CLK_ENA));
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 << AIF2_MOD_CLK_EN),
						    (0x0 << AIF2_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 << AIF2_MOD_RST_CTL),
						    (0x0 << AIF2_MOD_RST_CTL));
				/*DISABLE systemclk */
				if (sunxi_codec->aif1_clken == 0
				    && sunxi_codec->aif3_clken == 0)
					snd_soc_update_bits(codec,
							    SUNXI_SYSCLK_CTL,
							    (0x1 << SYSCLK_ENA),
							    (0x0 <<
							     SYSCLK_ENA));
			}
		}
		break;
	}
	mutex_unlock(&sunxi_codec->aifclk_mutex);
	return 0;
}

int ac_aif3clk(struct snd_soc_dapm_widget *w,
	       struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);
	mutex_lock(&sunxi_codec->aifclk_mutex);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		if (sunxi_codec->aif2_clken == 0) {
			/*enable AIF2CLK */
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x1 << AIF2CLK_ENA),
					    (0x1 << AIF2CLK_ENA));
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << AIF2_MOD_CLK_EN),
					    (0x1 << AIF2_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << AIF2_MOD_RST_CTL),
					    (0x1 << AIF2_MOD_RST_CTL));
			/*enable systemclk */
			if (sunxi_codec->aif1_clken == 0
			    && sunxi_codec->aif3_clken == 0)
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << SYSCLK_ENA),
						    (0x1 << SYSCLK_ENA));
		}
		sunxi_codec->aif2_clken++;
		if (sunxi_codec->aif3_clken == 0) {
			/*enable AIF3CLK */
			snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
					    (0x1 << AIF3_MOD_CLK_EN),
					    (0x1 << AIF3_MOD_CLK_EN));
			snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
					    (0x1 << AIF3_MOD_RST_CTL),
					    (0x1 << AIF3_MOD_RST_CTL));
		}
		sunxi_codec->aif3_clken++;

		break;
	case SND_SOC_DAPM_POST_PMD:
		if (sunxi_codec->aif2_clken > 0) {
			sunxi_codec->aif2_clken--;
			if (sunxi_codec->aif2_clken == 0) {
				/*disable AIF2CLK */
				snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
						    (0x1 << AIF2CLK_ENA),
						    (0x0 << AIF2CLK_ENA));
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 << AIF2_MOD_CLK_EN),
						    (0x0 << AIF2_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 << AIF2_MOD_RST_CTL),
						    (0x0 << AIF2_MOD_RST_CTL));
				/*DISABLE systemclk */
				if (sunxi_codec->aif1_clken == 0
				    && sunxi_codec->aif3_clken == 0)
					snd_soc_update_bits(codec,
							    SUNXI_SYSCLK_CTL,
							    (0x1 << SYSCLK_ENA),
							    (0x0 <<
							     SYSCLK_ENA));
			}
		}
		if (sunxi_codec->aif3_clken > 0) {
			sunxi_codec->aif3_clken--;
			if (sunxi_codec->aif3_clken == 0) {
				/*enable AIF3CLK */
				snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
						    (0x1 << AIF3_MOD_CLK_EN),
						    (0x0 << AIF3_MOD_CLK_EN));
				snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
						    (0x1 << AIF3_MOD_RST_CTL),
						    (0x0 << AIF3_MOD_RST_CTL));
			}
		}

		break;
	}
	mutex_unlock(&sunxi_codec->aifclk_mutex);
	return 0;
}

static int aif2inl_vir_event(struct snd_soc_dapm_widget *w,
			     struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_update_bits(codec, SUNXI_AIF3_SGP_CTRL,
				    (0x3 << AIF2_DAC_SRC),
				    (0x1 << AIF2_DAC_SRC));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_update_bits(codec, SUNXI_AIF3_SGP_CTRL,
				    (0x3 << AIF2_DAC_SRC),
				    (0x0 << AIF2_DAC_SRC));
		break;
	}
	return 0;
}

static int aif2inr_vir_event(struct snd_soc_dapm_widget *w,
			     struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_update_bits(codec, SUNXI_AIF3_SGP_CTRL,
				    (0x3 << AIF2_DAC_SRC),
				    (0x2 << AIF2_DAC_SRC));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_update_bits(codec, SUNXI_AIF3_SGP_CTRL,
				    (0x3 << AIF2_DAC_SRC),
				    (0x0 << AIF2_DAC_SRC));
		break;
	}
	return 0;
}

/*
*	use for enable src function
*/
static int set_src_function(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct sunxi_codec_priv *sunxi_internal_codec =
	    snd_soc_codec_get_drvdata(codec);

	src_function_en = ucontrol->value.integer.value[0];
	if (src_function_en) {
		pr_debug("Enable src clk , config src 8k-8k.\n");
		/*enable srcclk */
		clk_prepare_enable(sunxi_internal_codec->srcclk);
		/*src1 */
		snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
				    (0x1 << SRC1_MOD_CLK_EN),
				    (0x1 << SRC1_MOD_CLK_EN));
		/*src2 */
		snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
				    (0x1 << SRC2_MOD_CLK_EN),
				    (0x1 << SRC2_MOD_CLK_EN));
		/*src2 */
		snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
				    (0x1 << SRC2_MOD_RST_CTL),
				    (0x1 << SRC2_MOD_RST_CTL));
		/*src1 */
		snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
				    (0x1 << SRC1_MOD_RST_CTL),
				    (0x1 << SRC1_MOD_RST_CTL));

		/*select src1 source from aif2 */
		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC1_SRC),
				    (0x1 << SRC1_SRC));
		/*select src2 source from aif2 */
		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC2_SRC),
				    (0x1 << SRC2_SRC));
		/*enable src1 */
		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC1_ENA),
				    (0x1 << SRC1_ENA));
		/*enable src2 */
		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC1_ENA),
				    (0x1 << SRC1_ENA));
	} else {
		pr_debug("disable src clk.\n");
		clk_disable_unprepare(sunxi_internal_codec->srcclk);
		/*src1 */
		snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
				    (0x1 << SRC1_MOD_CLK_EN),
				    (0x0 << SRC1_MOD_CLK_EN));
		/*src2 */
		snd_soc_update_bits(codec, SUNXI_MOD_CLK_ENA,
				    (0x1 << SRC2_MOD_CLK_EN),
				    (0x0 << SRC2_MOD_CLK_EN));
		/*src2 */
		snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
				    (0x1 << SRC2_MOD_RST_CTL),
				    (0x0 << SRC2_MOD_RST_CTL));
		/*src1 */
		snd_soc_update_bits(codec, SUNXI_MOD_RST_CTL,
				    (0x1 << SRC1_MOD_RST_CTL),
				    (0x0 << SRC1_MOD_RST_CTL));

		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC1_ENA),
				    (0x0 << SRC1_ENA));
		snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL, (0x1 << SRC1_ENA),
				    (0x0 << SRC1_ENA));
	}

	return 0;
}

static int get_src_function(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = src_function_en;

	return 0;
}

static const DECLARE_TLV_DB_SCALE(headphone_vol_tlv, -6300, 100, 0);
static const DECLARE_TLV_DB_SCALE(lineout_vol_tlv, -4800, 150, 0);
static const DECLARE_TLV_DB_SCALE(phoneout_vol_tlv, -450, 150, 0);

static const DECLARE_TLV_DB_SCALE(aif1_ad_slot0_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif1_ad_slot1_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif1_da_slot0_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif1_da_slot1_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif1_ad_slot0_mix_vol_tlv, -600, 600, 0);
static const DECLARE_TLV_DB_SCALE(aif1_ad_slot1_mix_vol_tlv, -600, 600, 0);

static const DECLARE_TLV_DB_SCALE(aif2_ad_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif2_da_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(aif2_ad_mix_vol_tlv, -600, 600, 0);

static const DECLARE_TLV_DB_SCALE(adc_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(dac_vol_tlv, -11925, 75, 0);
static const DECLARE_TLV_DB_SCALE(dig_vol_tlv, -7308, 116, 0);
static const DECLARE_TLV_DB_SCALE(dac_mix_vol_tlv, -600, 600, 0);
static const DECLARE_TLV_DB_SCALE(adc_input_vol_tlv, -450, 150, 0);

/*mic1/mic2: 0db when 000, and from 24db to 42db when 001 to 111*/
static const DECLARE_TLV_DB_SCALE(mic1_boost_vol_tlv, 0, 300, 0);
static const DECLARE_TLV_DB_SCALE(mic2_boost_vol_tlv, 0, 300, 0);
static const DECLARE_TLV_DB_SCALE(phonein_vol_tlv, -450, 150, 0);
static const DECLARE_TLV_DB_SCALE(linein_to_l_r_mix_vol_tlv, -450, 150, 0);
static const DECLARE_TLV_DB_SCALE(phonein_pre_amplifier_vol_tlv, -450, 150, 0);

static const struct snd_kcontrol_new sunxi_codec_controls[] = {
	/*output devices vol controls */
	SOC_SINGLE_TLV("headphone volume", HP_VOLC, HPVOL, 0x3f, 0,
		       headphone_vol_tlv),
	SOC_SINGLE_TLV("lineout volume", LINEOUT_VOLC, LINEOUTVOL, 0x1f, 0,
		       lineout_vol_tlv),
	SOC_SINGLE_TLV("phoneout volume", PHONEOUT_CTRL, PHONEOUTG, 0x7, 0,
		       phoneout_vol_tlv),

	/*input devices gain controls */
	SOC_SINGLE_TLV("MIC1 boost amplifier gain", MIC1G_MICBIAS_CTRL,
		       MIC1BOOST, 0x7, 0, mic1_boost_vol_tlv),
	SOC_SINGLE_TLV("MIC2 boost amplifier gain", MIC2G_LINEEN_CTRL,
		       MIC2BOOST, 0x7, 0, mic2_boost_vol_tlv),
	SOC_SINGLE_TLV("LINEINL/R to L_R output mixer gain", LINEIN_GCTRL,
		       LINEING, 0x7, 0, linein_to_l_r_mix_vol_tlv),
	SOC_DOUBLE_TLV("phonein pre-amplifier gain", PHONEIN_GCTRL, PHONENG,
		       PHONEPG, 0x7, 0, phonein_pre_amplifier_vol_tlv),
	SOC_SINGLE_TLV("phonein(p-n) to L_R output mixer gain", LINEIN_GCTRL,
		       PHONEG, 0x7, 0, phonein_pre_amplifier_vol_tlv),

	/*aif1 vol control */
	SOC_DOUBLE_TLV("AIF1 ADC timeslot 0 volume", SUNXI_AIF1_VOL_CTRL1,
		       AIF1_AD0L_VOL, AIF1_AD0R_VOL, 0xff, 0,
		       aif1_ad_slot0_vol_tlv),
	SOC_DOUBLE_TLV("AIF1 ADC timeslot 1 volume", SUNXI_AIF1_VOL_CTRL2,
		       AIF1_AD1L_VOL, AIF1_AD1R_VOL, 0xff, 0,
		       aif1_ad_slot1_vol_tlv),
	SOC_DOUBLE_TLV("AIF1 DAC timeslot 0 volume", SUNXI_AIF1_VOL_CTRL3,
		       AIF1_DA0L_VOL, AIF1_DA0R_VOL, 0xff, 0,
		       aif1_da_slot0_vol_tlv),
	SOC_DOUBLE_TLV("AIF1 DAC timeslot 1 volume", SUNXI_AIF1_VOL_CTRL4,
		       AIF1_DA1L_VOL, AIF1_DA1R_VOL, 0xff, 0,
		       aif1_da_slot1_vol_tlv),
	SOC_DOUBLE_TLV("AIF1 ADC timeslot 0 mixer gain", SUNXI_AIF1_MXR_GAIN,
		       AIF1_AD0L_MXR_GAIN, AIF1_AD0R_MXR_GAIN, 0xf, 0,
		       aif1_ad_slot0_mix_vol_tlv),
	SOC_DOUBLE_TLV("AIF1 ADC timeslot 1 mixer gain", SUNXI_AIF1_MXR_GAIN,
		       AIF1_AD1L_MXR_GAIN, AIF1_AD1R_MXR_GAIN, 0x3, 0,
		       aif1_ad_slot1_mix_vol_tlv),

	/*AIF2 vol control */
	SOC_DOUBLE_TLV("AIF2 ADC volume", SUNXI_AIF2_VOL_CTRL1, AIF2_ADCL_VOL,
		       AIF2_ADCR_VOL, 0xff, 0, aif2_ad_vol_tlv),
	SOC_DOUBLE_TLV("AIF2 DAC volume", SUNXI_AIF2_VOL_CTRL2, AIF2_DACL_VOL,
		       AIF2_DACR_VOL, 0xff, 0, aif2_da_vol_tlv),
	SOC_DOUBLE_TLV("AIF2 ADC mixer gain", SUNXI_AIF2_MXR_GAIN,
		       AIF2_ADCL_MXR_GAIN, AIF2_ADCR_MXR_GAIN, 0xf, 0,
		       aif2_ad_mix_vol_tlv),

	/*adc vol control */
	SOC_DOUBLE_TLV("ADC volume", SUNXI_ADC_VOL_CTRL, ADC_VOL_L, ADC_VOL_R,
		       0xff, 0, adc_vol_tlv),

	/*dac vol control */
	SOC_DOUBLE_TLV("DAC volume", SUNXI_DAC_VOL_CTRL, DAC_VOL_L, DAC_VOL_R,
		       0xff, 0, dac_vol_tlv),
	SOC_DOUBLE_TLV("DAC mixer gain", SUNXI_DAC_MXR_GAIN, DACL_MXR_GAIN,
		       DACR_MXR_GAIN, 0xf, 0, dac_mix_vol_tlv),
	SOC_SINGLE_TLV("digital volume", SUNXI_DAC_DBG_CTRL, DVC, 0x3f, 0,
		       dig_vol_tlv),

	/*ADC input gain */
	SOC_SINGLE_TLV("ADC input gain", ADC_AP_EN, ADCG, 0x7, 0,
		       adc_input_vol_tlv),
	SOC_SINGLE_BOOL_EXT("SRC FUCTION", 0, get_src_function,
			    set_src_function),
};

/*AIF1 AD0 OUT */
static const char *aif1out0l_text[] = {
	"AIF1_AD0L", "AIF1_AD0R", "SUM_AIF1AD0L_AIF1AD0R",
	"AVE_AIF1AD0L_AIF1AD0R"
};
static const char *aif1out0r_text[] = {
	"AIF1_AD0R", "AIF1_AD0L", "SUM_AIF1AD0L_AIF1AD0R",
	"AVE_AIF1AD0L_AIF1AD0R"
};

static const struct soc_enum aif1out0l_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD0L_SRC, 4, aif1out0l_text);

static const struct snd_kcontrol_new aif1out0l_mux =
SOC_DAPM_ENUM("AIF1OUT0L Mux", aif1out0l_enum);

static const struct soc_enum aif1out0r_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD0R_SRC, 4, aif1out0r_text);

static const struct snd_kcontrol_new aif1out0r_mux =
SOC_DAPM_ENUM("AIF1OUT0R Mux", aif1out0r_enum);

/*AIF1 AD1 OUT */
static const char *aif1out1l_text[] = {
	"AIF1_AD1L", "AIF1_AD1R", "SUM_AIF1ADC1L_AIF1ADC1R",
	"AVE_AIF1ADC1L_AIF1ADC1R"
};
static const char *aif1out1r_text[] = {
	"AIF1_AD1R", "AIF1_AD1L", "SUM_AIF1ADC1L_AIF1ADC1R",
	"AVE_AIF1ADC1L_AIF1ADC1R"
};

static const struct soc_enum aif1out1l_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD1L_SRC, 4, aif1out1l_text);

static const struct snd_kcontrol_new aif1out1l_mux =
SOC_DAPM_ENUM("AIF1OUT1L Mux", aif1out1l_enum);

static const struct soc_enum aif1out1r_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD1R_SRC, 4, aif1out1r_text);

static const struct snd_kcontrol_new aif1out1r_mux =
SOC_DAPM_ENUM("AIF1OUT1R Mux", aif1out1r_enum);

/*AIF1 DA0 IN*/
static const char *aif1in0l_text[] = {
	"AIF1_DA0L", "AIF1_DA0R", "SUM_AIF1DA0L_AIF1DA0R",
	"AVE_AIF1DA0L_AIF1DA0R"
};
static const char *aif1in0r_text[] = {
	"AIF1_DA0R", "AIF1_DA0L", "SUM_AIF1DA0L_AIF1DA0R",
	"AVE_AIF1DA0L_AIF1DA0R"
};

static const struct soc_enum aif1in0l_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_DACDAT_CTRL, AIF1_DA0L_SRC, 4, aif1in0l_text);

static const struct snd_kcontrol_new aif1in0l_mux =
SOC_DAPM_ENUM("AIF1IN0L Mux", aif1in0l_enum);

static const struct soc_enum aif1in0r_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_DACDAT_CTRL, AIF1_DA0R_SRC, 4, aif1in0r_text);

static const struct snd_kcontrol_new aif1in0r_mux =
SOC_DAPM_ENUM("AIF1IN0R Mux", aif1in0r_enum);

/*AIF1 DA1 IN*/
static const char *aif1in1l_text[] = {
	"AIF1_DA1L", "AIF1_DA1R", "SUM_AIF1DA1L_AIF1DA1R",
	"AVE_AIF1DA1L_AIF1DA1R"
};
static const char *aif1in1r_text[] = {
	"AIF1_DA1R", "AIF1_DA1L", "SUM_AIF1DA1L_AIF1DA1R",
	"AVE_AIF1DA1L_AIF1DA1R"
};

static const struct soc_enum aif1in1l_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_DACDAT_CTRL, AIF1_DA1L_SRC, 4, aif1in1l_text);

static const struct snd_kcontrol_new aif1in1l_mux =
SOC_DAPM_ENUM("AIF1IN1L Mux", aif1in1l_enum);

static const struct soc_enum aif1in1r_enum =
SOC_ENUM_SINGLE(SUNXI_AIF1_DACDAT_CTRL, AIF1_DA1R_SRC, 4, aif1in1r_text);

static const struct snd_kcontrol_new aif1in1r_mux =
SOC_DAPM_ENUM("AIF1IN1R Mux", aif1in1r_enum);

/*AIF1 AD0 MIXER SOURCE reg: 0x24c*/
static const struct snd_kcontrol_new aif1_ad0l_mxr_src_ctl[] = {
	SOC_DAPM_SINGLE("AIF1 DA0L Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0L_MXL_SRC_AIF1DA0L, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACL Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0L_MXL_SRC_AIF2DACL, 1, 0),
	SOC_DAPM_SINGLE("ADCL Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0L_MXL_SRC_ADCL, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACR Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0L_MXL_SRC_AIF2DACR, 1, 0),
};
static const struct snd_kcontrol_new aif1_ad0r_mxr_src_ctl[] = {
	SOC_DAPM_SINGLE("AIF1 DA0R Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0R_MXR_SRC_AIF1DA0R, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACR Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0R_MXR_SRC_AIF2DACR, 1, 0),
	SOC_DAPM_SINGLE("ADCR Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0R_MXR_SRC_ADCR, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACL Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD0R_MXR_SRC_AIF2DACL, 1, 0),
};

/*AIF1 AD1 MIXER SOURCE*/
static const struct snd_kcontrol_new aif1_ad1l_mxr_src_ctl[] = {
	SOC_DAPM_SINGLE("AIF2 DACL Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD1L_MXR_SRC_AIF2DACL, 1, 0),
	SOC_DAPM_SINGLE("ADCL Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD1L_MXR_SRC_ADCL, 1, 0),
};
static const struct snd_kcontrol_new aif1_ad1r_mxr_src_ctl[] = {
	SOC_DAPM_SINGLE("AIF2 DACR Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD1R_MXR_SRC_AIF2DACR, 1, 0),
	SOC_DAPM_SINGLE("ADCR Switch", SUNXI_AIF1_MXR_SRC,
			AIF1_AD1R_MXR_SRC_ADCR, 1, 0),
};

/*DAC mixer controls reg: 0x330*/
static const struct snd_kcontrol_new dacl_mxr_src_controls[] = {
	SOC_DAPM_SINGLE("ADCL Switch", SUNXI_DAC_MXR_SRC, DACL_MXR_SRC_ADCL, 1,
			0),
	SOC_DAPM_SINGLE("AIF2DACL Switch", SUNXI_DAC_MXR_SRC,
			DACL_MXR_SRC_AIF2DACL, 1, 0),
	SOC_DAPM_SINGLE("AIF1DA1L Switch", SUNXI_DAC_MXR_SRC,
			DACL_MXR_SRC_AIF1DA1L, 1, 0),
	SOC_DAPM_SINGLE("AIF1DA0L Switch", SUNXI_DAC_MXR_SRC,
			DACL_MXR_SRC_AIF1DA0L, 1, 0),
};
static const struct snd_kcontrol_new dacr_mxr_src_controls[] = {
	SOC_DAPM_SINGLE("ADCR Switch", SUNXI_DAC_MXR_SRC, DACR_MXR_SRC_ADCR, 1,
			0),
	SOC_DAPM_SINGLE("AIF2DACR Switch", SUNXI_DAC_MXR_SRC,
			DACR_MXR_SRC_AIF2DACR, 1, 0),
	SOC_DAPM_SINGLE("AIF1DA1R Switch", SUNXI_DAC_MXR_SRC,
			DACR_MXR_SRC_AIF1DA1R, 1, 0),
	SOC_DAPM_SINGLE("AIF1DA0R Switch", SUNXI_DAC_MXR_SRC,
			DACR_MXR_SRC_AIF1DA0R, 1, 0),
};

/*output mixer source select*/
/*defined left output mixer  areg: 0x01*/
static const struct snd_kcontrol_new codec_loutmix_controls[] = {
	SOC_DAPM_SINGLE("DACR Switch", LOMIXSC, LMIXMUTEDACR, 1, 0),
	SOC_DAPM_SINGLE("DACL Switch", LOMIXSC, LMIXMUTEDACL, 1, 0),
	SOC_DAPM_SINGLE("LINEINL Switch", LOMIXSC, LMIXMUTELINEINL, 1, 0),
	SOC_DAPM_SINGLE("PHONEN Switch", LOMIXSC, LMIXMUTEPHONEN, 1, 0),
	SOC_DAPM_SINGLE("PHONEP-PHONEN Switch", LOMIXSC, LMIXMUTEPHONEPN, 1, 0),
	SOC_DAPM_SINGLE("MIC2Booststage Switch", LOMIXSC, LMIXMUTEMIC2BOOST, 1,
			0),
	SOC_DAPM_SINGLE("MIC1Booststage Switch", LOMIXSC, LMIXMUTEMIC1BOOST, 1,
			0),
};

/*defined right output mixer areg:0x02*/
static const struct snd_kcontrol_new codec_routmix_controls[] = {
	SOC_DAPM_SINGLE("DACL Switch", ROMIXSC, RMIXMUTEDACL, 1, 0),
	SOC_DAPM_SINGLE("DACR Switch", ROMIXSC, RMIXMUTEDACR, 1, 0),
	SOC_DAPM_SINGLE("PHONEP-PHONEN Switch", ROMIXSC, RMIXMUTEPHONEPN, 1, 0),
	SOC_DAPM_SINGLE("PHONEP Switch", ROMIXSC, RMIXMUTEPHONEP, 1, 0),
	SOC_DAPM_SINGLE("LINEINR Switch", ROMIXSC, RMIXMUTELINEINR, 1, 0),
	SOC_DAPM_SINGLE("MIC2Booststage Switch", ROMIXSC, RMIXMUTEMIC2BOOST, 1,
			0),
	SOC_DAPM_SINGLE("MIC1Booststage Switch", ROMIXSC, RMIXMUTEMIC1BOOST, 1,
			0),
};

/*define phoneout mixer areg:0x08*/
static const struct snd_kcontrol_new codec_phoneoutmix_controls[] = {
	SOC_DAPM_SINGLE("Left Output Mixer Switch", PHONEOUT_CTRL, PHONEOUTS0,
			1, 0),
	SOC_DAPM_SINGLE("Right Output Mixer Switch", PHONEOUT_CTRL, PHONEOUTS1,
			1, 0),
	SOC_DAPM_SINGLE("MIC2Booststage Switch", PHONEOUT_CTRL, PHONEOUTS2, 1,
			0),
	SOC_DAPM_SINGLE("MIC1Booststage Switch", PHONEOUT_CTRL, PHONEOUTS3, 1,
			0),
};

/*hp source select*/
/*headphone input source  areg:0x03*/
static const char *codec_hp_r_func_sel[] = {
	"DACR HPR Switch", "Right Analog Mixer HPR Switch"
};
static const struct soc_enum codec_hp_r_func_enum =
SOC_ENUM_SINGLE(DAC_PA_SRC, RHPIS, 2, codec_hp_r_func_sel);

static const struct snd_kcontrol_new codec_hp_r_func_controls =
SOC_DAPM_ENUM("HP_R Mux", codec_hp_r_func_enum);

static const char *codec_hp_l_func_sel[] = {
	"DACL HPL Switch", "Left Analog Mixer HPL Switch"
};
static const struct soc_enum codec_hp_l_func_enum =
SOC_ENUM_SINGLE(DAC_PA_SRC, LHPIS, 2, codec_hp_l_func_sel);

static const struct snd_kcontrol_new codec_hp_l_func_controls =
SOC_DAPM_ENUM("HP_L Mux", codec_hp_l_func_enum);
#if 0
/*lineout source select areg:0x0a*/
static const char *codec_lineout_r_func_sel[] = {
	"MIXR Switch", "MIXL Switch"
};

static const struct soc_enum codec_lineout_r_func_enum =
SOC_ENUM_SINGLE(MIC2G_LINEEN_CTRL, LINEOUTR_SS, 2, codec_lineout_r_func_sel);

static const struct snd_kcontrol_new codec_lineout_r_func_controls =
SOC_DAPM_ENUM("Lineout_R Mux", codec_lineout_r_func_enum);

static const char *codec_lineout_l_func_sel[] = {
	"MIXL Switch", "MIXL MIXR Switch"
};

static const struct soc_enum codec_lineout_l_func_enum =
SOC_ENUM_SINGLE(MIC2G_LINEEN_CTRL, LINEOUTL_SS, 2, codec_lineout_l_func_sel);

static const struct snd_kcontrol_new codec_lineout_l_func_controls =
SOC_DAPM_ENUM("Lineout_L Mux", codec_lineout_l_func_enum);
#endif
/*AIF2 out  dreg:0x284*/
static const char *aif2outl_text[] = {
	"AIF2_ADCL", "AIF2_ADCR", "SUM_AIF2_ADCL_AIF2_ADCR",
	"AVE_AIF2_ADCL_AIF2_ADCR"
};
static const char *aif2outr_text[] = {
	"AIF2_ADCR", "AIF2_ADCL", "SUM_AIF2_ADCL_AIF2_ADCR",
	"AVE_AIF2_ADCL_AIF2_ADCR"
};

static const struct soc_enum aif2outl_enum =
SOC_ENUM_SINGLE(SUNXI_AIF2_ADCDAT_CTRL, AIF2_ADCL_SRC, 4, aif2outl_text);

static const struct snd_kcontrol_new aif2outl_mux =
SOC_DAPM_ENUM("AIF2OUTL Mux", aif2outl_enum);

static const struct soc_enum aif2outr_enum =
SOC_ENUM_SINGLE(SUNXI_AIF2_ADCDAT_CTRL, AIF2_ADCR_SRC, 4, aif2outr_text);

static const struct snd_kcontrol_new aif2outr_mux =
SOC_DAPM_ENUM("AIF2OUTR Mux", aif2outr_enum);

/*AIF2 IN dreg : 0x288*/
static const char *aif2inl_text[] = {
	"AIF2_DACL", "AIF2_DACR", "SUM_AIF2DACL_AIF2DACR",
	"AVE_AIF2DACL_AIF2DACR"
};
static const char *aif2inr_text[] = {
	"AIF2_DACR", "AIF2_DACL", "SUM_AIF2DACL_AIF2DACR",
	"AVE_AIF2DACL_AIF2DACR"
};

static const struct soc_enum aif2inl_enum =
SOC_ENUM_SINGLE(SUNXI_AIF2_DACDAT_CTRL, AIF2_DACL_SRC, 4, aif2inl_text);

static const struct snd_kcontrol_new aif2inl_mux =
SOC_DAPM_ENUM("AIF2INL Mux", aif2inl_enum);

static const struct soc_enum aif2inr_enum =
SOC_ENUM_SINGLE(SUNXI_AIF2_DACDAT_CTRL, AIF2_DACR_SRC, 4, aif2inr_text);

static const struct snd_kcontrol_new aif2inr_mux =
SOC_DAPM_ENUM("AIF2INR Mux", aif2inr_enum);

/*AIF2 output source select dreg:0x28c*/
static const struct snd_kcontrol_new aif2_adcl_mxr_src_controls[] = {
	SOC_DAPM_SINGLE("AIF1 DA0L Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCL_MXR_SRC_AIF1DA0L, 1, 0),
	SOC_DAPM_SINGLE("AIF1 DA1L Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCL_MXR_SRC_AIF1DA1L, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACR Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCL_MXR_SRC_AIF2DACR, 1, 0),
	SOC_DAPM_SINGLE("ADCL Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCL_MXR_SRC_ADCL, 1, 0),
};
static const struct snd_kcontrol_new aif2_adcr_mxr_src_controls[] = {
	SOC_DAPM_SINGLE("AIF1 DA0R Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCR_MXR_SRC_AIF1DA0R, 1, 0),
	SOC_DAPM_SINGLE("AIF1 DA1R Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCR_MXR_SRC_AIF1DA1R, 1, 0),
	SOC_DAPM_SINGLE("AIF2 DACL Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCR_MXR_SRC_AIF2DACL, 1, 0),
	SOC_DAPM_SINGLE("ADCR Switch", SUNXI_AIF2_MXR_SRC,
			AIF2_ADCR_MXR_SRC_ADCR, 1, 0),
};

/*aif3 output select dreg : 0x2cc*/
static const char *aif3out_text[] = {
	"NULL", "AIF2 ADC left channel", "AIF2 ADC right channel"
};

static const unsigned int aif3out_values[] = { 0, 1, 2 };

static const struct soc_enum aif3out_enum =
SOC_VALUE_ENUM_SINGLE(SUNXI_AIF3_SGP_CTRL, AIF3_ADC_SRC, 3,
		      ARRAY_SIZE(aif3out_text), aif3out_text, aif3out_values);

static const struct snd_kcontrol_new aif3out_mux =
SOC_DAPM_VALUE_ENUM("AIF3OUT Mux", aif3out_enum);

/*aif2 input source select dreg : */
static const char *aif2dacin_text[] = {
	"Left_s right_s AIF2", "Left_s AIF3 Right_s AIF2",
	"Left_s AIF2 Right_s AIF3"
};

static const struct soc_enum aif2dacin_enum =
SOC_ENUM_SINGLE(SUNXI_AIF3_SGP_CTRL, AIF2_DAC_SRC, 3, aif2dacin_text);

static const struct snd_kcontrol_new aif2dacin_mux =
SOC_DAPM_ENUM("AIF2 DAC SRC Mux", aif2dacin_enum);

/*ADC SOURCE SELECT*/
/*defined left input adc mixer*/
static const struct snd_kcontrol_new codec_ladcmix_controls[] = {
	SOC_DAPM_SINGLE("MIC1 boost Switch", LADCMIXSC, LADCMIXMUTEMIC1BOOST, 1,
			0),
	SOC_DAPM_SINGLE("MIC2 boost Switch", LADCMIXSC, LADCMIXMUTEMIC2BOOST, 1,
			0),
	SOC_DAPM_SINGLE("PHONEP-PHONEN Switch", LADCMIXSC, LADCMIXMUTEPHONEPN,
			1, 0),
	SOC_DAPM_SINGLE("PHONEN Switch", LADCMIXSC, LADCMIXMUTEPHONEN, 1, 0),
	SOC_DAPM_SINGLE("LINEINL Switch", LADCMIXSC, LADCMIXMUTELINEINL, 1, 0),
	SOC_DAPM_SINGLE("Lout_Mixer_Switch", LADCMIXSC, LADCMIXMUTELOUTPUT, 1,
			0),
	SOC_DAPM_SINGLE("Rout_Mixer_Switch", LADCMIXSC, LADCMIXMUTEROUTPUT, 1,
			0),
};

/*defined right input adc mixer*/
static const struct snd_kcontrol_new codec_radcmix_controls[] = {
	SOC_DAPM_SINGLE("MIC1 boost Switch", RADCMIXSC, RADCMIXMUTEMIC1BOOST, 1,
			0),
	SOC_DAPM_SINGLE("MIC2 boost Switch", RADCMIXSC, RADCMIXMUTEMIC2BOOST, 1,
			0),
	SOC_DAPM_SINGLE("PHONEP-PHONEN Switch", RADCMIXSC, RADCMIXMUTEPHONEPN,
			1, 0),
	SOC_DAPM_SINGLE("PHONEP Switch", RADCMIXSC, RADCMIXMUTEPHONEP, 1, 0),
	SOC_DAPM_SINGLE("LINEINR Switch", RADCMIXSC, RADCMIXMUTELINEINR, 1, 0),
	SOC_DAPM_SINGLE("Rout_Mixer_Switch", RADCMIXSC, RADCMIXMUTEROUTPUT, 1,
			0),
	SOC_DAPM_SINGLE("Lout_Mixer_Switch", RADCMIXSC, RADCMIXMUTELOUTPUT, 1,
			0),
};

/*mic2 source select*/
static const char *mic2src_text[] = {
	"MIC3", "MIC2"
};

static const struct soc_enum mic2src_enum =
SOC_ENUM_SINGLE(MIC1G_MICBIAS_CTRL, MIC2_SS, 2, mic2src_text);

static const struct snd_kcontrol_new mic2src_mux =
SOC_DAPM_ENUM("MIC2 SRC", mic2src_enum);

 /*DMIC*/ static const char *adc_mux_text[] = {
	"ADC",
	"DMIC",
};
static const struct soc_enum adc_enum = SOC_ENUM_SINGLE(0, 0, 2, adc_mux_text);
static const struct snd_kcontrol_new adcl_mux =
SOC_DAPM_ENUM_VIRT("ADCL Mux", adc_enum);
static const struct snd_kcontrol_new adcr_mux =
SOC_DAPM_ENUM_VIRT("ADCR Mux", adc_enum);

/*vir widget: it just to control aif3 input*/
static const struct snd_kcontrol_new aif2inl_aif2switch =
SOC_DAPM_SINGLE("aif2inl aif2", SUNXI_AIF1_RXD_CTRL, 8, 1, 0);
static const struct snd_kcontrol_new aif2inr_aif2switch =
SOC_DAPM_SINGLE("aif2inr aif2", SUNXI_AIF1_RXD_CTRL, 9, 1, 0);

static const struct snd_kcontrol_new aif2inl_aif3switch =
SOC_DAPM_SINGLE("aif2inl aif3", SUNXI_AIF1_RXD_CTRL, 10, 1, 0);
static const struct snd_kcontrol_new aif2inr_aif3switch =
SOC_DAPM_SINGLE("aif2inr aif3", SUNXI_AIF1_RXD_CTRL, 11, 1, 0);

/*built widget*/
static const struct snd_soc_dapm_widget ac_dapm_widgets[] = {
	/*aif1  input interface */
	SND_SOC_DAPM_AIF_IN_E("AIF1DACL", "AIF1 Playback", 0, SND_SOC_NOPM, 0,
			      0, ac_aif1clk,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_AIF_IN_E("AIF1DACR", "AIF1 Playback", 0, SND_SOC_NOPM, 0,
			      0, ac_aif1clk,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_MUX("AIF1IN0L Mux", SUNXI_AIF1_DACDAT_CTRL, AIF1_DA0L_ENA,
			 0, &aif1in0l_mux),
	SND_SOC_DAPM_MUX("AIF1IN0R Mux", SUNXI_AIF1_DACDAT_CTRL, AIF1_DA0R_ENA,
			 0, &aif1in0r_mux),

	SND_SOC_DAPM_MUX("AIF1IN1L Mux", SUNXI_AIF1_DACDAT_CTRL, AIF1_DA1L_ENA,
			 0, &aif1in1l_mux),
	SND_SOC_DAPM_MUX("AIF1IN1R Mux", SUNXI_AIF1_DACDAT_CTRL, AIF1_DA1R_ENA,
			 0, &aif1in1r_mux),

	SND_SOC_DAPM_MIXER_E("DACL Mixer", DAC_PA_SRC, DACALEN, 0,
			     dacl_mxr_src_controls,
			     ARRAY_SIZE(dacl_mxr_src_controls),
			     late_enable_dac,
			     SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_MIXER_E("DACR Mixer", DAC_PA_SRC, DACAREN, 0,
			     dacr_mxr_src_controls,
			     ARRAY_SIZE(dacr_mxr_src_controls),
			     late_enable_dac,
			     SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_MIXER("Left Output Mixer", DAC_PA_SRC, LMIXEN, 0,
			   codec_loutmix_controls,
			   ARRAY_SIZE(codec_loutmix_controls)),
	SND_SOC_DAPM_MIXER("Right Output Mixer", DAC_PA_SRC, RMIXEN, 0,
			   codec_routmix_controls,
			   ARRAY_SIZE(codec_routmix_controls)),

	SND_SOC_DAPM_MUX("HP_R Mux", SND_SOC_NOPM, 0, 0,
			 &codec_hp_r_func_controls),
	SND_SOC_DAPM_MUX("HP_L Mux", SND_SOC_NOPM, 0, 0,
			 &codec_hp_l_func_controls),

	SND_SOC_DAPM_OUTPUT("HPOUTL"),
	SND_SOC_DAPM_OUTPUT("HPOUTR"),
	SND_SOC_DAPM_OUTPUT("LINEOUTL"),
	SND_SOC_DAPM_OUTPUT("LINEOUTR"),
	/*output devices */
	/*headphone */
	SND_SOC_DAPM_HP("Headphone", ac_headphone_event),

	SND_SOC_DAPM_MIXER("Phoneout Mixer", PHONEOUT_CTRL, PHONEOUT_EN, 0,
			   codec_phoneoutmix_controls,
			   ARRAY_SIZE(codec_phoneoutmix_controls)),

	SND_SOC_DAPM_OUTPUT("PHONEOUTP"),
	SND_SOC_DAPM_OUTPUT("PHONEOUTN"),

	/*aif2 input interface */
	SND_SOC_DAPM_AIF_IN_E("AIF2DACL", "AIF2 Playback", 0, SND_SOC_NOPM, 0,
			      0, ac_aif2clk,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_AIF_IN_E("AIF2DACR", "AIF2 Playback", 0, SND_SOC_NOPM, 0,
			      0, ac_aif2clk,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_SWITCH("AIF2INL Mux switch", SND_SOC_NOPM, 0, 1,
			    &aif2inl_aif2switch),
	SND_SOC_DAPM_SWITCH("AIF2INR Mux switch", SND_SOC_NOPM, 0, 1,
			    &aif2inr_aif2switch),

	SND_SOC_DAPM_SWITCH("AIF2INL Mux VIR switch", SND_SOC_NOPM, 0, 1,
			    &aif2inl_aif3switch),
	SND_SOC_DAPM_SWITCH("AIF2INR Mux VIR switch", SND_SOC_NOPM, 0, 1,
			    &aif2inr_aif3switch),

	SND_SOC_DAPM_MUX("AIF2INL Mux", SUNXI_AIF2_DACDAT_CTRL, AIF2_DACL_ENA,
			 0, &aif2inl_mux),
	SND_SOC_DAPM_MUX("AIF2INR Mux", SUNXI_AIF2_DACDAT_CTRL, AIF2_DACR_ENA,
			 0, &aif2inr_mux),
	/*aif2 switch */
	SND_SOC_DAPM_PGA("AIF2INL_VIR", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("AIF2INR_VIR", SND_SOC_NOPM, 0, 0, NULL, 0),

	/*aif2 output interface */
	SND_SOC_DAPM_AIF_OUT_E("AIF2ADCL", "AIF2 Capture", 0, SND_SOC_NOPM, 0,
			       0, ac_aif2clk,
			       SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_AIF_OUT_E("AIF2ADCR", "AIF2 Capture", 0, SND_SOC_NOPM, 0,
			       0, ac_aif2clk,
			       SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_MUX("AIF2OUTL Mux", SUNXI_AIF2_ADCDAT_CTRL, AIF2_ADCL_EN,
			 0, &aif2outl_mux),
	SND_SOC_DAPM_MUX("AIF2OUTR Mux", SUNXI_AIF2_ADCDAT_CTRL, AIF2_ADCR_EN,
			 0, &aif2outr_mux),

	SND_SOC_DAPM_MIXER("AIF2 ADL Mixer", SND_SOC_NOPM, 0, 0,
			   aif2_adcl_mxr_src_controls,
			   ARRAY_SIZE(aif2_adcl_mxr_src_controls)),
	SND_SOC_DAPM_MIXER("AIF2 ADR Mixer", SND_SOC_NOPM, 0, 0,
			   aif2_adcr_mxr_src_controls,
			   ARRAY_SIZE(aif2_adcr_mxr_src_controls)),

	/*aif3 input interface */
	SND_SOC_DAPM_AIF_IN_E("AIF3IN", "AIF3 Playback", 0, SND_SOC_NOPM, 0, 0,
			      ac_aif3clk,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	/*virtual widget */
	SND_SOC_DAPM_PGA_E("AIF2INL Mux VIR", SND_SOC_NOPM, 0, 0, NULL, 0,
			   aif2inl_vir_event,
			   SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_PGA_E("AIF2INR Mux VIR", SND_SOC_NOPM, 0, 0, NULL, 0,
			   aif2inr_vir_event,
			   SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	/*aif3 output interface */
	SND_SOC_DAPM_AIF_OUT_E("AIF3OUT", "AIF3 Capture", 0, SND_SOC_NOPM, 0, 0,
			       ac_aif3clk,
			       SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_MUX("AIF3OUT Mux", SND_SOC_NOPM, 0, 0, &aif3out_mux),

	/*aif1 output interface */
	SND_SOC_DAPM_AIF_OUT_E("AIF1ADCL", "AIF1 Capture", 0, SND_SOC_NOPM, 0,
			       0, ac_aif1clk,
			       SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_AIF_OUT_E("AIF1ADCR", "AIF1 Capture", 0, SND_SOC_NOPM, 0,
			       0, ac_aif1clk,
			       SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_MUX("AIF1OUT0L Mux", SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD0L_ENA,
			 0, &aif1out0l_mux),
	SND_SOC_DAPM_MUX("AIF1OUT0R Mux", SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD0R_ENA,
			 0, &aif1out0r_mux),

	SND_SOC_DAPM_MUX("AIF1OUT1L Mux", SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD1L_ENA,
			 0, &aif1out1l_mux),
	SND_SOC_DAPM_MUX("AIF1OUT1R Mux", SUNXI_AIF1_ADCDAT_CTRL, AIF1_AD1R_ENA,
			 0, &aif1out1r_mux),

	SND_SOC_DAPM_MIXER("AIF1 AD0L Mixer", SND_SOC_NOPM, 0, 0,
			   aif1_ad0l_mxr_src_ctl,
			   ARRAY_SIZE(aif1_ad0l_mxr_src_ctl)),
	SND_SOC_DAPM_MIXER("AIF1 AD0R Mixer", SND_SOC_NOPM, 0, 0,
			   aif1_ad0r_mxr_src_ctl,
			   ARRAY_SIZE(aif1_ad0r_mxr_src_ctl)),

	SND_SOC_DAPM_MIXER("AIF1 AD1L Mixer", SND_SOC_NOPM, 0, 0,
			   aif1_ad1l_mxr_src_ctl,
			   ARRAY_SIZE(aif1_ad1l_mxr_src_ctl)),
	SND_SOC_DAPM_MIXER("AIF1 AD1R Mixer", SND_SOC_NOPM, 0, 0,
			   aif1_ad1r_mxr_src_ctl,
			   ARRAY_SIZE(aif1_ad1r_mxr_src_ctl)),

	SND_SOC_DAPM_MIXER_E("LEFT ADC input Mixer", ADC_AP_EN, ADCLEN, 0,
			     codec_ladcmix_controls,
			     ARRAY_SIZE(codec_ladcmix_controls),
			     late_enable_adc,
			     SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_MIXER_E("RIGHT ADC input Mixer", ADC_AP_EN, ADCREN, 0,
			     codec_radcmix_controls,
			     ARRAY_SIZE(codec_radcmix_controls),
			     late_enable_adc,
			     SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_PGA("MIC1 PGA", MIC1G_MICBIAS_CTRL, MIC1AMPEN, 0, NULL, 0),
	SND_SOC_DAPM_PGA("MIC2 PGA", MIC2G_LINEEN_CTRL, MIC2AMPEN, 0, NULL, 0),

	SND_SOC_DAPM_PGA("PHONEIN PGA", SND_SOC_NOPM, 0, 0, NULL, 0),

	SND_SOC_DAPM_MUX("MIC2 SRC", SND_SOC_NOPM, 0, 0, &mic2src_mux),

	SND_SOC_DAPM_MICBIAS("MainMic Bias", MIC1G_MICBIAS_CTRL, MMICBIASEN, 0),
	SND_SOC_DAPM_MICBIAS("HMic Bias", MIC1G_MICBIAS_CTRL, HMICBIASEN, 0),

	/*INPUT widget */
	SND_SOC_DAPM_INPUT("MIC1P"),
	SND_SOC_DAPM_INPUT("MIC1N"),

	SND_SOC_DAPM_INPUT("MIC2"),
	SND_SOC_DAPM_INPUT("MIC3"),

	SND_SOC_DAPM_INPUT("LINEINL"),
	SND_SOC_DAPM_INPUT("LINEINR"),
	SND_SOC_DAPM_INPUT("PHONEINP"),
	SND_SOC_DAPM_INPUT("PHONEINN"),
	 /*DMIC*/
	SND_SOC_DAPM_VIRT_MUX("ADCL Mux", SND_SOC_NOPM, 0, 0, &adcl_mux),
	SND_SOC_DAPM_VIRT_MUX("ADCR Mux", SND_SOC_NOPM, 0, 0, &adcr_mux),

};

static const struct snd_soc_dapm_route ac_dapm_routes[] = {
/*aif1 playback route*/
	/*AIF1 DA0 IN */
	{"AIF1IN0L Mux", "AIF1_DA0L", "AIF1DACL"},
	{"AIF1IN0L Mux", "AIF1_DA0R", "AIF1DACR"},

	{"AIF1IN0R Mux", "AIF1_DA0R", "AIF1DACR"},
	{"AIF1IN0R Mux", "AIF1_DA0L", "AIF1DACL"},

	/*AIF1 DA1 IN */
	{"AIF1IN1L Mux", "AIF1_DA1L", "AIF1DACL"},
	{"AIF1IN1L Mux", "AIF1_DA1R", "AIF1DACR"},

	{"AIF1IN1R Mux", "AIF1_DA1R", "AIF1DACR"},
	{"AIF1IN1R Mux", "AIF1_DA1L", "AIF1DACL"},

	/*DAC mixer */
	{"DACL Mixer", "AIF1DA0L Switch", "AIF1IN0L Mux"},
	{"DACL Mixer", "AIF1DA1L Switch", "AIF1IN1L Mux"},
	{"DACL Mixer", "ADCL Switch", "ADCL Mux"},
	{"DACL Mixer", "AIF2DACL Switch", "AIF2INL_VIR"},
	{"DACR Mixer", "AIF1DA0R Switch", "AIF1IN0R Mux"},
	{"DACR Mixer", "AIF1DA1R Switch", "AIF1IN1R Mux"},
	{"DACR Mixer", "ADCR Switch", "ADCR Mux"},
	{"DACR Mixer", "AIF2DACR Switch", "AIF2INR_VIR"},

	/*left output mixer */
	{"Left Output Mixer", "DACL Switch", "DACL Mixer"},
	{"Left Output Mixer", "DACR Switch", "DACR Mixer"},
	{"Left Output Mixer", "LINEINL Switch", "LINEINL"},
	{"Left Output Mixer", "PHONEN Switch", "PHONEINN"},
	{"Left Output Mixer", "PHONEP-PHONEN Switch", "PHONEIN PGA"},
	{"Left Output Mixer", "MIC2Booststage Switch", "MIC2 PGA"},
	{"Left Output Mixer", "MIC1Booststage Switch", "MIC1 PGA"},

	/*right output mixer */
	{"Right Output Mixer", "DACR Switch", "DACR Mixer"},
	{"Right Output Mixer", "DACL Switch", "DACL Mixer"},
	{"Right Output Mixer", "LINEINR Switch", "LINEINR"},
	{"Right Output Mixer", "PHONEP Switch", "PHONEINP"},
	{"Right Output Mixer", "PHONEP-PHONEN Switch", "PHONEIN PGA"},
	{"Right Output Mixer", "MIC2Booststage Switch", "MIC2 PGA"},
	{"Right Output Mixer", "MIC1Booststage Switch", "MIC1 PGA"},

	/*hp mux */
	{"HP_R Mux", "DACR HPR Switch", "DACR Mixer"},
	{"HP_R Mux", "Right Analog Mixer HPR Switch", "Right Output Mixer"},

	{"HP_L Mux", "DACL HPL Switch", "DACL Mixer"},
	{"HP_L Mux", "Left Analog Mixer HPL Switch", "Left Output Mixer"},

	/*hp endpoint */
	{"HPOUTR", NULL, "HP_R Mux"},
	{"HPOUTL", NULL, "HP_L Mux"},

	{"Headphone", NULL, "HPOUTR"},
	{"Headphone", NULL, "HPOUTL"},

/*aif1 capture route*/
	{"AIF1ADCL", NULL, "AIF1OUT0L Mux"},
	{"AIF1ADCR", NULL, "AIF1OUT0R Mux"},

	{"AIF1ADCL", NULL, "AIF1OUT1L Mux"},
	{"AIF1ADCR", NULL, "AIF1OUT1R Mux"},

	/* aif1out0 mux 11---13 */
	{"AIF1OUT0L Mux", "AIF1_AD0L", "AIF1 AD0L Mixer"},
	{"AIF1OUT0L Mux", "AIF1_AD0R", "AIF1 AD0R Mixer"},

	{"AIF1OUT0R Mux", "AIF1_AD0R", "AIF1 AD0R Mixer"},
	{"AIF1OUT0R Mux", "AIF1_AD0L", "AIF1 AD0L Mixer"},

	/*AIF1OUT1 mux 11--13 */
	{"AIF1OUT1L Mux", "AIF1_AD1L", "AIF1 AD1L Mixer"},
	{"AIF1OUT1L Mux", "AIF1_AD1R", "AIF1 AD1R Mixer"},

	{"AIF1OUT1R Mux", "AIF1_AD1R", "AIF1 AD1R Mixer"},
	{"AIF1OUT1R Mux", "AIF1_AD1L", "AIF1 AD1L Mixer"},

	/*AIF1 AD0L Mixer */
	{"AIF1 AD0L Mixer", "AIF1 DA0L Switch", "AIF1IN0L Mux"},
	{"AIF1 AD0L Mixer", "AIF2 DACL Switch", "AIF2INL_VIR"},
	{"AIF1 AD0L Mixer", "ADCL Switch", "ADCL Mux"},
	{"AIF1 AD0L Mixer", "AIF2 DACR Switch", "AIF2INR_VIR"},

	/*AIF1 AD0R Mixer */
	{"AIF1 AD0R Mixer", "AIF1 DA0R Switch", "AIF1IN0R Mux"},
	{"AIF1 AD0R Mixer", "AIF2 DACR Switch", "AIF2INR_VIR"},
	{"AIF1 AD0R Mixer", "ADCR Switch", "ADCR Mux"},
	{"AIF1 AD0R Mixer", "AIF2 DACL Switch", "AIF2INL_VIR"},

	/*AIF1 AD1L Mixer */
	{"AIF1 AD1L Mixer", "AIF2 DACL Switch", "AIF2INL_VIR"},
	{"AIF1 AD1L Mixer", "ADCL Switch", "ADCL Mux"},
	/*AIF1 AD1R Mixer */
	{"AIF1 AD1R Mixer", "AIF2 DACR Switch", "AIF2INR_VIR"},
	{"AIF1 AD1R Mixer", "ADCR Switch", "ADCR Mux"},

	/*aif2 virtual */
	{"AIF2INL Mux switch", "aif2inl aif2", "AIF2INL Mux"},
	{"AIF2INR Mux switch", "aif2inr aif2", "AIF2INR Mux"},

	{"AIF2INL_VIR", NULL, "AIF2INL Mux switch"},
	{"AIF2INR_VIR", NULL, "AIF2INR Mux switch"},

	{"AIF2INL_VIR", NULL, "AIF2INL Mux VIR"},
	{"AIF2INR_VIR", NULL, "AIF2INR Mux VIR"},

	/*LADC SOURCE mixer */
	{"LEFT ADC input Mixer", "MIC1 boost Switch", "MIC1 PGA"},
	{"LEFT ADC input Mixer", "MIC2 boost Switch", "MIC2 PGA"},
	{"LEFT ADC input Mixer", "LINEINL Switch", "LINEINL"},
	{"LEFT ADC input Mixer", "PHONEP-PHONEN Switch", "PHONEIN PGA"},
	{"LEFT ADC input Mixer", "PHONEN Switch", "PHONEINN"},
	{"LEFT ADC input Mixer", "Lout_Mixer_Switch", "Left Output Mixer"},
	{"LEFT ADC input Mixer", "Rout_Mixer_Switch", "Right Output Mixer"},

	/*RADC SOURCE mixer */
	{"RIGHT ADC input Mixer", "MIC1 boost Switch", "MIC1 PGA"},
	{"RIGHT ADC input Mixer", "MIC2 boost Switch", "MIC2 PGA"},
	{"RIGHT ADC input Mixer", "LINEINR Switch", "LINEINR"},
	{"RIGHT ADC input Mixer", "PHONEP-PHONEN Switch", "PHONEIN PGA"},
	{"RIGHT ADC input Mixer", "PHONEP Switch", "PHONEINP"},
	{"RIGHT ADC input Mixer", "Rout_Mixer_Switch", "Right Output Mixer"},
	{"RIGHT ADC input Mixer", "Lout_Mixer_Switch", "Left Output Mixer"},

	/*ADC--ADCMUX */
	{"ADCR Mux", NULL, "RIGHT ADC input Mixer"},
	{"ADCL Mux", NULL, "LEFT ADC input Mixer"},

	{"MIC1 PGA", NULL, "MIC1P"},
	{"MIC1 PGA", NULL, "MIC1N"},

	{"MIC2 PGA", NULL, "MIC2 SRC"},

	{"MIC2 SRC", "MIC2", "MIC2"},
	{"MIC2 SRC", "MIC3", "MIC3"},

	{"PHONEIN PGA", NULL, "PHONEINN"},
	{"PHONEIN PGA", NULL, "PHONEINP"},

	/*phoneout */
	{"Phoneout Mixer", "MIC2Booststage Switch", "MIC1 PGA"},
	{"Phoneout Mixer", "MIC2Booststage Switch", "MIC2 PGA"},
	{"Phoneout Mixer", "Right Output Mixer Switch", "Right Output Mixer"},
	{"Phoneout Mixer", "Left Output Mixer Switch", "Left Output Mixer"},

	{"PHONEOUTP", NULL, "Phoneout Mixer"},
	{"PHONEOUTN", NULL, "Phoneout Mixer"},

/*aif2 capture route*/
	{"AIF2ADCL", NULL, "AIF2OUTL Mux"},
	{"AIF2ADCR", NULL, "AIF2OUTR Mux"},

	{"AIF2OUTL Mux", "AIF2_ADCL", "AIF2 ADL Mixer"},
	{"AIF2OUTL Mux", "AIF2_ADCR", "AIF2 ADR Mixer"},

	{"AIF2OUTR Mux", "AIF2_ADCR", "AIF2 ADR Mixer"},
	{"AIF2OUTR Mux", "AIF2_ADCL", "AIF2 ADL Mixer"},

	{"AIF2 ADL Mixer", "AIF1 DA0L Switch", "AIF1IN0L Mux"},
	{"AIF2 ADL Mixer", "AIF1 DA1L Switch", "AIF1IN1L Mux"},
	{"AIF2 ADL Mixer", "AIF2 DACR Switch", "AIF2INR_VIR"},
	{"AIF2 ADL Mixer", "ADCL Switch", "ADCL Mux"},

	{"AIF2 ADR Mixer", "AIF1 DA0R Switch", "AIF1IN0R Mux"},
	{"AIF2 ADR Mixer", "AIF1 DA1R Switch", "AIF1IN1R Mux"},
	{"AIF2 ADR Mixer", "AIF2 DACL Switch", "AIF2INL_VIR"},
	{"AIF2 ADR Mixer", "ADCR Switch", "ADCR Mux"},

/*aif2 playback route*/
	{"AIF2INL Mux", "AIF2_DACL", "AIF2DACL"},
	{"AIF2INL Mux", "AIF2_DACR", "AIF2DACR"},

	{"AIF2INR Mux", "AIF2_DACR", "AIF2DACR"},
	{"AIF2INR Mux", "AIF2_DACL", "AIF2DACL"},

/*aif3 playback route*/
	{"AIF2INL Mux VIR switch", "aif2inl aif3", "AIF3IN"},
	{"AIF2INR Mux VIR switch", "aif2inr aif3", "AIF3IN"},

	{"AIF2INL Mux VIR", NULL, "AIF2INL Mux VIR switch"},
	{"AIF2INR Mux VIR", NULL, "AIF2INR Mux VIR switch"},

/*aif3 capture route*/
	{"AIF3OUT", NULL, "AIF3OUT Mux"},
	{"AIF3OUT Mux", "AIF2 ADC left channel", "AIF2 ADL Mixer"},
	{"AIF3OUT Mux", "AIF2 ADC right channel", "AIF2 ADR Mixer"},
};

struct aif1_fs {
	unsigned int samplerate;
	int aif1_bclk_div;
	int aif1_srbit;
};

struct aif1_lrck {
	int aif1_lrlk_div;
	int aif1_lrlk_bit;
};

struct aif1_word_size {
	int aif1_wsize_val;
	int aif1_wsize_bit;
};

static const struct aif1_fs codec_aif1_fs[] = {
	{44100, 4, 7},
	{48000, 4, 8},
	{8000, 9, 0},
	{11025, 8, 1},
	{12000, 8, 2},
	{16000, 7, 3},
	{22050, 6, 4},
	{24000, 6, 5},
	{32000, 5, 6},
	{96000, 2, 9},
	{192000, 1, 10},
};

static const struct aif1_lrck codec_aif1_lrck[] = {
	{16, 0},
	{32, 1},
	{64, 2},
	{128, 3},
	{256, 4},
};

static const struct aif1_word_size codec_aif1_wsize[] = {
	{8, 0},
	{16, 1},
	{20, 2},
	{24, 3},
};

static int codec_aif_mute(struct snd_soc_dai *codec_dai, int mute)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);
	mutex_lock(&sunxi_codec->aifclk_mutex);
	if (mute) {
		snd_soc_write(codec, SUNXI_DAC_VOL_CTRL, 0);
	} else {
		snd_soc_write(codec, SUNXI_DAC_VOL_CTRL, 0xa0a0);
	}
	mutex_unlock(&sunxi_codec->aifclk_mutex);
	return 0;
}

static void codec_aif_shutdown(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *codec_dai)
{
	return;
}
static int codec_hw_params(struct snd_pcm_substream *substream,
			   struct snd_pcm_hw_params *params,
			   struct snd_soc_dai *codec_dai)
{
	int i = 0;
	u32 AIF_CLK_CTRL = 0;
	int aif1_word_size = 16;
	int aif1_lrlk_div = 64;
	int bclk_div_factor = 0;
	struct snd_soc_codec *codec = codec_dai->codec;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);

	switch (codec_dai->id) {
	case 1:
		AIF_CLK_CTRL = SUNXI_AIF1_CLK_CTRL;
		if (sunxi_codec->aif1_lrlk_div == 0)
			aif1_lrlk_div = 64;
		else
			aif1_lrlk_div = sunxi_codec->aif1_lrlk_div;
		break;
	case 2:
		AIF_CLK_CTRL = SUNXI_AIF2_CLK_CTRL;
		if (sunxi_codec->aif2_lrlk_div == 0)
			aif1_lrlk_div = 64;
		else
			aif1_lrlk_div = sunxi_codec->aif2_lrlk_div;
		break;
	default:
		return -EINVAL;
	}

	/* FIXME make up the codec_aif1_lrck factor
	 * adjust for more working scene
	 */
	switch (aif1_lrlk_div) {
	case 16:
		bclk_div_factor = 4;
		break;
	case 32:
		bclk_div_factor = 2;
		break;
	case 64:
		bclk_div_factor = 0;
		break;
	case 128:
		bclk_div_factor = -2;
		break;
	case 256:
		bclk_div_factor = -4;
		break;
	default:
		pr_err("invalid lrlk_div setting in sysconfig!\n");
		return -EINVAL;
	}

	for (i = 0; i < ARRAY_SIZE(codec_aif1_lrck); i++) {
		if (codec_aif1_lrck[i].aif1_lrlk_div == aif1_lrlk_div) {
			snd_soc_update_bits(codec, AIF_CLK_CTRL,
					    (0x7 << AIF1_LRCK_DIV),
					    ((codec_aif1_lrck[i].
					      aif1_lrlk_bit) << AIF1_LRCK_DIV));
			break;
		}
	}

	for (i = 0; i < ARRAY_SIZE(codec_aif1_fs); i++) {
		if (codec_aif1_fs[i].samplerate == params_rate(params)) {
			snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL,
					    (0xf << AIF1_FS),
					    ((codec_aif1_fs[i].
					      aif1_srbit) << AIF1_FS));
			snd_soc_update_bits(codec, SUNXI_SYS_SR_CTRL,
					    (0xf << AIF2_FS),
					    ((codec_aif1_fs[i].
					      aif1_srbit) << AIF2_FS));
			bclk_div_factor += codec_aif1_fs[i].aif1_bclk_div;
			snd_soc_update_bits(codec, AIF_CLK_CTRL,
					    (0xf << AIF1_BCLK_DIV),
					    ((bclk_div_factor) <<
					     AIF1_BCLK_DIV));
			break;
		}
	}
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
		aif1_word_size = 24;
		break;
	case SNDRV_PCM_FORMAT_S16_LE:
	default:
		aif1_word_size = 16;
		break;
	}

	if (params_channels(params) == 1
	    && (AIF_CLK_CTRL == SUNXI_AIF2_CLK_CTRL))
		snd_soc_update_bits(codec, AIF_CLK_CTRL, (0x1 << DSP_MONO_PCM),
				    (0x1 << DSP_MONO_PCM));
	else
		snd_soc_update_bits(codec, AIF_CLK_CTRL, (0x1 << DSP_MONO_PCM),
				    (0x0 << DSP_MONO_PCM));

	for (i = 0; i < ARRAY_SIZE(codec_aif1_wsize); i++) {
		if (codec_aif1_wsize[i].aif1_wsize_val == aif1_word_size) {
			snd_soc_update_bits(codec, AIF_CLK_CTRL,
					    (0x3 << AIF1_WORD_SIZ),
					    ((codec_aif1_wsize[i].
					      aif1_wsize_bit) <<
					     AIF1_WORD_SIZ));
			break;
		}
	}
	return 0;
}
static int codec_start(struct snd_pcm_substream *substream,
		       struct snd_soc_dai *codec_dai)
{
	return 0;
}

static int codec_set_dai_sysclk(struct snd_soc_dai *codec_dai,
				int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;

	switch (clk_id) {
	case AIF1_CLK:
		/*system clk from aif1 */
		pr_debug("set AIF1_CLK \n");
		snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
				    (0x1 << SYSCLK_SRC), (0x0 << SYSCLK_SRC));
		break;
	case AIF2_CLK:
		/*system clk from aif2 */
		pr_debug("set AIF2_CLK \n");
		snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
				    (0x1 << SYSCLK_SRC), (0x1 << SYSCLK_SRC));
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int codec_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	int reg_val;
	u32 AIF_CLK_CTRL = 0;
	struct snd_soc_codec *codec = codec_dai->codec;

	switch (codec_dai->id) {
	case 1:
		AIF_CLK_CTRL = SUNXI_AIF1_CLK_CTRL;
		break;
	case 2:
		AIF_CLK_CTRL = SUNXI_AIF2_CLK_CTRL;
		break;
	default:
		return -EINVAL;
	}

	/*
	 *      master or slave selection
	 *      0 = Master mode
	 *      1 = Slave mode
	 */
	reg_val = snd_soc_read(codec, AIF_CLK_CTRL);
	reg_val &= ~(0x1 << AIF1_MSTR_MOD);
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:	/* codec clk & frm master, ap is slave */
		reg_val |= (0x0 << AIF1_MSTR_MOD);
		break;
	case SND_SOC_DAIFMT_CBS_CFS:	/* codec clk & frm slave,ap is master */
		reg_val |= (0x1 << AIF1_MSTR_MOD);
		break;
	default:
		pr_err("unknwon master/slave format\n");
		return -EINVAL;
	}
	snd_soc_write(codec, AIF_CLK_CTRL, reg_val);

	/* i2s mode selection */
	reg_val = snd_soc_read(codec, AIF_CLK_CTRL);
	reg_val &= ~(3 << AIF1_DATA_FMT);
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:	/* I2S1 mode */
		reg_val |= (0x0 << AIF1_DATA_FMT);
		break;
	case SND_SOC_DAIFMT_RIGHT_J:	/* Right Justified mode */
		reg_val |= (0x2 << AIF1_DATA_FMT);
		break;
	case SND_SOC_DAIFMT_LEFT_J:	/* Left Justified mode */
		reg_val |= (0x1 << AIF1_DATA_FMT);
		break;
	case SND_SOC_DAIFMT_DSP_A:	/* L reg_val msb after FRM LRC */
		reg_val |= (0x3 << AIF1_DATA_FMT);
		break;
	default:
		pr_err("%s, line:%d\n", __func__, __LINE__);
		return -EINVAL;
	}
	snd_soc_write(codec, AIF_CLK_CTRL, reg_val);

	/* DAI signal inversions */
	reg_val = snd_soc_read(codec, AIF_CLK_CTRL);
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:	/* normal bit clock + nor frame */
		reg_val &= ~(0x1 << AIF1_LRCK_INV);
		reg_val &= ~(0x1 << AIF1_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_NB_IF:	/* normal bclk + inv frm */
		reg_val |= (0x1 << AIF1_LRCK_INV);
		reg_val &= ~(0x1 << AIF1_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_IB_NF:	/* invert bclk + nor frm */
		reg_val &= ~(0x1 << AIF1_LRCK_INV);
		reg_val |= (0x1 << AIF1_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_IB_IF:	/* invert bclk + inv frm */
		reg_val |= (0x1 << AIF1_LRCK_INV);
		reg_val |= (0x1 << AIF1_BCLK_INV);
		break;
	}
	snd_soc_write(codec, AIF_CLK_CTRL, reg_val);

	return 0;
}

static int codec_set_fll(struct snd_soc_dai *codec_dai, int pll_id, int source,
			 unsigned int freq_in, unsigned int freq_out)
{
	struct snd_soc_codec *codec = codec_dai->codec;

	if (!freq_out)
		return 0;
	if ((freq_in < 128000) || (freq_in > 24576000)) {
		return -EINVAL;
	} else if ((freq_in == 24576000) || (freq_in == 22579200)) {
		switch (pll_id) {
		case PLLCLK:
			/*select aif1/aif2 clk source from pll */
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x3 << AIF1CLK_SRC),
					    (0x3 << AIF1CLK_SRC));
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x3 << AIF2CLK_SRC),
					    (0x3 << AIF2CLK_SRC));
			break;
		case MCLK:
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x3 << AIF1CLK_SRC),
					    (0x0 << AIF1CLK_SRC));
			snd_soc_update_bits(codec, SUNXI_SYSCLK_CTL,
					    (0x3 << AIF2CLK_SRC),
					    (0x0 << AIF2CLK_SRC));
		default:
			return -EINVAL;
		}
		return 0;
	}

	return 0;
}

static int codec_aif3_set_dai_fmt(struct snd_soc_dai *codec_dai,
				  unsigned int fmt)
{
	int reg_val;
	struct snd_soc_codec *codec = codec_dai->codec;

	/* DAI signal inversions */
	reg_val = snd_soc_read(codec, SUNXI_AIF3_CLK_CTRL);
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:	/* normal bit clock + nor frame */
		reg_val &= ~(0x1 << AIF3_LRCK_INV);
		reg_val &= ~(0x1 << AIF3_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_NB_IF:	/* normal bclk + inv frm */
		reg_val |= (0x1 << AIF3_LRCK_INV);
		reg_val &= ~(0x1 << AIF3_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_IB_NF:	/* invert bclk + nor frm */
		reg_val &= ~(0x1 << AIF3_LRCK_INV);
		reg_val |= (0x1 << AIF3_BCLK_INV);
		break;
	case SND_SOC_DAIFMT_IB_IF:	/* invert bclk + inv frm */
		reg_val |= (0x1 << AIF3_LRCK_INV);
		reg_val |= (0x1 << AIF3_BCLK_INV);
		break;
	}
	snd_soc_write(codec, SUNXI_AIF3_CLK_CTRL, reg_val);

	return 0;
}

static int codec_aif3_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *codec_dai)
{
	int aif3_word_size = 0;
	int aif3_size = 0;
	struct snd_soc_codec *codec = codec_dai->codec;

	/*0x2c0 config aif3clk from aif2clk */
	snd_soc_update_bits(codec, SUNXI_AIF3_CLK_CTRL, (0x3 << AIF3_CLOC_SRC),
			    (0x1 << AIF3_CLOC_SRC));
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S24_LE:
		aif3_word_size = 24;
		aif3_size = 3;
		break;
	case SNDRV_PCM_FORMAT_S16_LE:
	default:
		aif3_word_size = 16;
		aif3_size = 1;
		break;
	}
	snd_soc_update_bits(codec, SUNXI_AIF3_CLK_CTRL, (0x3 << AIF3_WORD_SIZ),
			    aif3_size << AIF3_WORD_SIZ);

	return 0;
}

static int codec_set_bias_level(struct snd_soc_codec *codec,
				enum snd_soc_bias_level level)
{
	switch (level) {
	case SND_SOC_BIAS_ON:
		pr_debug("%s,line:%d, SND_SOC_BIAS_ON\n", __func__, __LINE__);
		break;
	case SND_SOC_BIAS_PREPARE:
		pr_debug("%s,line:%d, SND_SOC_BIAS_PREPARE\n", __func__,
			 __LINE__);
		break;
	case SND_SOC_BIAS_STANDBY:
		/*on */
		pr_debug("%s,line:%d, SND_SOC_BIAS_STANDBY\n", __func__,
			 __LINE__);
		break;
	case SND_SOC_BIAS_OFF:
		/*off */
		pr_debug("%s,line:%d, SND_SOC_BIAS_OFF\n", __func__, __LINE__);
		break;
	}
	codec->dapm.bias_level = level;

	return 0;
}

static const struct snd_soc_dai_ops codec_aif1_dai_ops = {
	.startup = codec_start,
	.set_sysclk = codec_set_dai_sysclk,
	.set_fmt = codec_set_dai_fmt,
	.hw_params = codec_hw_params,
	.shutdown = codec_aif_shutdown,
	.digital_mute = codec_aif_mute,
	.set_pll = codec_set_fll,
};

static const struct snd_soc_dai_ops codec_aif2_dai_ops = {
	.set_sysclk = codec_set_dai_sysclk,
	.set_fmt = codec_set_dai_fmt,
	.hw_params = codec_hw_params,
	.digital_mute = codec_aif_mute,
	.set_pll = codec_set_fll,
};

static const struct snd_soc_dai_ops codec_aif3_dai_ops = {
	.hw_params = codec_aif3_hw_params,
	.set_fmt = codec_aif3_set_dai_fmt,
};

static struct snd_soc_dai_driver codec_dai[] = {
	{
	 .name = "codec-aif1",
	 .id = 1,
	 .playback = {
		      .stream_name = "AIF1 Playback",
		      .channels_min = 1,
		      .channels_max = 2,
		      .rates = codec_RATES,
		      .formats = codec_FORMATS,
		      },
	 .capture = {
		     .stream_name = "AIF1 Capture",
		     .channels_min = 1,
		     .channels_max = 2,
		     .rates = codec_RATES,
		     .formats = codec_FORMATS,
		     },
	 .ops = &codec_aif1_dai_ops,
	 },
	{
	 .name = "codec-aif2",
	 .id = 2,
	 .playback = {
		      .stream_name = "AIF2 Playback",
		      .channels_min = 1,
		      .channels_max = 2,
		      .rates = codec_RATES,
		      .formats = codec_FORMATS,
		      },
	 .capture = {
		     .stream_name = "AIF2 Capture",
		     .channels_min = 1,
		     .channels_max = 2,
		     .rates = codec_RATES,
		     .formats = codec_FORMATS,
		     },
	 .ops = &codec_aif2_dai_ops,
	 },
	{
	 .name = "codec-aif3",
	 .id = 3,
	 .playback = {
		      .stream_name = "AIF3 Playback",
		      .channels_min = 1,
		      .channels_max = 1,
		      .rates = codec_RATES,
		      .formats = codec_FORMATS,
		      },
	 .capture = {
		     .stream_name = "AIF3 Capture",
		     .channels_min = 1,
		     .channels_max = 1,
		     .rates = codec_RATES,
		     .formats = codec_FORMATS,
		     },
	 .ops = &codec_aif3_dai_ops,
	 }
};

static int codec_soc_probe(struct snd_soc_codec *codec)
{
	int ret = 0;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct sunxi_codec_priv *sunxi_codec = snd_soc_codec_get_drvdata(codec);

	sunxi_codec->codec = codec;
	sunxi_codec->dac_enable = 0;
	sunxi_codec->adc_enable = 0;
	sunxi_codec->aif1_clken = 0;
	sunxi_codec->aif2_clken = 0;
	sunxi_codec->aif3_clken = 0;
	mutex_init(&sunxi_codec->dac_mutex);
	mutex_init(&sunxi_codec->adc_mutex);
	mutex_init(&sunxi_codec->aifclk_mutex);
	/* Add virtual switch */
	ret = snd_soc_add_codec_controls(codec, sunxi_codec_controls,
					 ARRAY_SIZE(sunxi_codec_controls));
	if (ret) {
		pr_err("[audio-codec] Failed to register audio mode control, "
		       "will continue without it.\n");
	}
	snd_soc_dapm_new_controls(dapm, ac_dapm_widgets,
				  ARRAY_SIZE(ac_dapm_widgets));
	snd_soc_dapm_add_routes(dapm, ac_dapm_routes,
				ARRAY_SIZE(ac_dapm_routes));
	codec_init(sunxi_codec);

	return 0;
}

/* power down chip */
static int codec_soc_remove(struct snd_soc_codec *codec)
{
	return 0;
}

static unsigned int codec_read(struct snd_soc_codec *codec, unsigned int reg)
{
	if (reg <= 0x1e) {
		/*analog reg */
		return read_prcm_wvalue(reg);
	} else {
		/*digital reg */
		return readl(baseaddr + reg);
	}
}

static int codec_write(struct snd_soc_codec *codec,
		       unsigned int reg, unsigned int value)
{
	if (reg <= 0x1e) {
		/*analog reg */
		write_prcm_wvalue(reg, value);
	} else {
		/*digital reg */
		writel(value, baseaddr + reg);
	}
	return 0;
}

static int codec_suspend(struct snd_soc_codec *codec)
{
	char pin_name[SUNXI_PIN_NAME_MAX_LEN];
	unsigned long config;
	struct sunxi_codec_priv *sunxi_internal_codec =
	    snd_soc_codec_get_drvdata(codec);
	pr_debug("[audio codec]:suspend start.\n");
	if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
		sunxi_gpio_to_name(sunxi_internal_codec->audio_pa_en.gpio.gpio,
				   pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
	}

	if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
		sunxi_gpio_to_name(sunxi_internal_codec->audio_pa_ctrl.gpio.
				   gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
	}

	if (sunxi_internal_codec->aif2_used) {
		aif_pin_request(2, 0);
	}
	if (sunxi_internal_codec->aif3_used) {
		aif_pin_request(3, 0);
	}
	if (src_function_en) {
		clk_disable_unprepare(sunxi_internal_codec->srcclk);
	}

	pr_debug("[audio codec]:suspend end..\n");

	return 0;
}

static int codec_resume(struct snd_soc_codec *codec)
{
	struct sunxi_codec_priv *sunxi_internal_codec =
	    snd_soc_codec_get_drvdata(codec);
	pr_debug("[audio codec]:resume start\n");

	if (src_function_en) {
		if (clk_prepare_enable(sunxi_internal_codec->srcclk)) {
			pr_err
			    ("open sunxi_internal_codec->srcclk failed! line = %d\n",
			     __LINE__);
		}
	}

	if (sunxi_internal_codec->aif2_used) {
		aif_pin_request(2, 1);
	}
	if (sunxi_internal_codec->aif3_used) {
		aif_pin_request(3, 1);
	}

	codec_init(sunxi_internal_codec);

	pr_debug("[audio codec]:resume end..\n");
	return 0;
}
static struct snd_soc_codec_driver soc_codec_dev_codec = {
	.probe = codec_soc_probe,
	.remove = codec_soc_remove,
	.suspend = codec_suspend,
	.resume = codec_resume,
	.set_bias_level = codec_set_bias_level,
	.read = codec_read,
	.write = codec_write,
	.ignore_pmdown_time = 1,
};

static int sunxi_internal_codec_probe(struct platform_device *pdev)
{
	s32 ret = 0;
	struct sunxi_codec_priv *sunxi_internal_codec;
	script_item_u val;
	script_item_value_type_e type;
	int req_status;

	sunxi_internal_codec =
	    devm_kzalloc(&pdev->dev, sizeof(struct sunxi_codec_priv),
			 GFP_KERNEL);
	if (!sunxi_internal_codec) {
		dev_err(&pdev->dev, "Can't allocate sunxi_codec_priv\n");
		ret = -ENOMEM;
		goto err0;
	}
	dev_set_drvdata(&pdev->dev, sunxi_internal_codec);

	/* get clk */
	sunxi_internal_codec->srcclk = clk_get(NULL, "pll_audiox4");
	if (IS_ERR(sunxi_internal_codec->srcclk)) {
		dev_err(&pdev->dev, "[audio-codec]Can't get src clocks\n");
		ret = PTR_ERR(sunxi_internal_codec->srcclk);
		goto err1;
	}

	type = script_get_item("audio0", "headphone_vol", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] headphone_vol type err!\n");
	} else {
		sunxi_internal_codec->gain_config.headphonevol = val.val;
	}

	type = script_get_item("audio0", "main_mic_vol", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] main_mic_vol type err!\n");
	} else {
		sunxi_internal_codec->gain_config.maingain = val.val;
	}

	type = script_get_item("audio0", "headset_mic_vol", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] headset_mic_vol  type err!\n");
	} else {
		sunxi_internal_codec->gain_config.headsetmicgain = val.val;
	}

	type = script_get_item("audio0", "aif2_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] aif2_used type err!\n");
	} else {
		sunxi_internal_codec->aif2_used = val.val;
	}

	type = script_get_item("audio0", "aif3_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] aif3_used type err!\n");
	} else {
		sunxi_internal_codec->aif3_used = val.val;
	}

	type = script_get_item("audio0", "pa_gpio_reverse", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] pa_gpio_reverse type err!\n");
	} else {
		sunxi_internal_codec->pa_gpio_reverse = val.val;
	}

	type = script_get_item("audio0", "audio_pa_en",
			    &sunxi_internal_codec->audio_pa_en);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		sunxi_internal_codec->audio_pa_en.gpio.gpio = -1;
		pr_err("[ audio ] err:try to get audio_pa_en failed!\n");
	}

	req_status = gpio_request(sunxi_internal_codec->audio_pa_en.gpio.gpio, NULL);
	if (0 != req_status) {
		pr_err("request gpio failed!\n");
	}

	type = script_get_item("audio0", "audio_pa_ctrl", &sunxi_internal_codec->audio_pa_ctrl);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		sunxi_internal_codec->audio_pa_ctrl.gpio.gpio = -1;
		pr_err("[ audio ] err:try to get audio_pa_ctrl failed!\n");
	}

	type = script_get_item("audio0", "aif1_lrlk_div", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] aif1_lrlk_div err!\n");
		sunxi_internal_codec->aif1_lrlk_div = 0;
	} else {
		sunxi_internal_codec->aif1_lrlk_div = val.val;
	}

	type = script_get_item("codec_aif2", "aif2_lrlk_div", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		pr_err("[audiocodec] aif2_used type err!\n");
		sunxi_internal_codec->aif2_lrlk_div = 0;
	} else {
		sunxi_internal_codec->aif2_lrlk_div = val.val;
	}

	req_status = gpio_request(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, NULL);
	if (0 != req_status) {
		pr_err("request gpio failed!\n");
	}

	snd_soc_register_codec(&pdev->dev, &soc_codec_dev_codec, codec_dai,
			       ARRAY_SIZE(codec_dai));

	ret = sysfs_create_group(&pdev->dev.kobj, &audio_debug_attr_group);
	if (ret) {
		pr_err("[audio-codec]failed to create attr group\n");
	}
	return 0;

err1:
	devm_kfree(&pdev->dev, sunxi_internal_codec);
err0:
	return ret;
}

static int __exit sunxi_internal_codec_remove(struct platform_device *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &audio_debug_attr_group);
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static void sunxi_internal_codec_shutdown(struct platform_device *pdev)
{
	struct sunxi_codec_priv *sunxi_internal_codec = dev_get_drvdata(&pdev->dev);

	if (gpio_is_valid(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio)) {
			if (sunxi_internal_codec->pa_gpio_reverse) {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 1);
			} else {
				gpio_set_value(sunxi_internal_codec->audio_pa_ctrl.gpio.gpio, 0);
			}
		}
	usleep_range(2000, 3000);
	if (gpio_is_valid(sunxi_internal_codec->audio_pa_en.gpio.gpio)) {
		gpio_set_value(sunxi_internal_codec->audio_pa_en.gpio.gpio, 0);
	}

	return;
}

static struct platform_device sunxi_internal_codec_device = {
	.name = "sunxi-internal-codec",
	.id = -1,
};

static struct platform_driver sunxi_internal_codec_driver = {
	.driver = {
		   .name = "sunxi-internal-codec",
		   .owner = THIS_MODULE,
		   },
	.probe = sunxi_internal_codec_probe,
	.remove = __exit_p(sunxi_internal_codec_remove),
	.shutdown = sunxi_internal_codec_shutdown,
};

static int __init sndpcm_codec_init(void)
{
	int err = 0;
	err = platform_device_register(&sunxi_internal_codec_device);
	if (err < 0)
		return err;

	err = platform_driver_register(&sunxi_internal_codec_driver);
	if (err < 0)
		return err;

	return 0;
}

module_init(sndpcm_codec_init);

static void __exit sndpcm_codec_exit(void)
{
	platform_driver_unregister(&sunxi_internal_codec_driver);
}
module_exit(sndpcm_codec_exit);

MODULE_DESCRIPTION("codec ALSA soc codec driver");
MODULE_AUTHOR("guoyingyang<guoyingyang@allwinnertech.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sunxi-internal-codec");
