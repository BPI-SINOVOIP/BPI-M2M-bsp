/*
 * sound\soc\sunxi\daudio\sunxi-daudio.c
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <asm/dma.h>
#include <mach/sunxi-smc.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include <linux/gpio.h>
#include "sunxi-daudiodma0.h"
#include "sunxi-daudio0.h"

struct sunxi_daudio_info sunxi_daudio;
static struct pinctrl *daudio_pinctrl;
static script_item_u  *pin_daudio0_list;
static unsigned int pin_count		= 0;
static bool  daudio0_loop_en 		= false;
static int regsave[11];
#ifdef CONFIG_ARCH_SUN9IW1
static struct clk *daudio_pll3clk 		= NULL;
#else
#ifdef CONFIG_ARCH_SUN8IW7
static struct clk *daudio_pll			= NULL;
#else
static struct clk *daudio_pll2clk 		= NULL;
static struct clk *daudio_pllx8 		= NULL;
#endif
#endif
static struct clk *daudio_moduleclk	= NULL;

static struct sunxi_dma_params sunxi_daudio_pcm_stereo_out = {
	.name		= "daudio_play",
	.dma_addr	= SUNXI_DAUDIOBASE + SUNXI_DAUDIOTXFIFO,/*send data address	*/
};

static struct sunxi_dma_params sunxi_daudio_pcm_stereo_in = {
	.name   	= "daudio_capture",
	.dma_addr	=SUNXI_DAUDIOBASE + SUNXI_DAUDIORXFIFO,/*accept data address	*/
};

static const struct _i2s0_bclk_div_s i2s_bclk_div_s[] = {
	{16, 22579200, 44100, 1411200, 16, 7},
	{16, 22579200, 22050, 705600, 32, 9},
	{16, 22579200, 11025, 352800, 64, 11},
	{16, 22579200, 88200, 2822400, 8, 5},
	{16, 22579200, 176400, 5644800, 4, 3},

	{32, 22579200, 44100, 2822400, 8, 5},
	{32, 22579200, 22050, 1411200, 16, 7},
	{32, 22579200, 11025, 705600, 32, 9},
	{32, 22579200, 88200, 5644800, 4, 3},
	{32, 22579200, 176400, 11289600, 2, 2},

	{64, 22579200, 44100, 5644800, 4, 3},
	{64, 22579200, 22050, 2822400, 8, 5},
	{64, 22579200, 11025, 1411200, 16, 7},
	{64, 22579200, 88200, 11289600, 2, 2},
	{64, 22579200, 176400, 22579200, 1, 1},

	{128, 22579200, 44100, 11289600, 2, 2},
	{128, 22579200, 22050, 5644800, 4, 3},
	{128, 22579200, 11025, 2822400, 8, 5},
	{128, 22579200, 88200, 22579200, 1, 1},

	{256, 22579200, 44100, 22579200, 1, 1},
	{256, 22579200, 22050, 11289600, 2, 2},
	{256, 22579200, 11025, 5644800, 4, 3},

	{512, 22579200, 22050, 22579200, 1, 1},
	{512, 22579200, 11025, 11289600, 2, 2},

	{1024, 22579200, 11025, 22579200, 1, 1},

	{16, 24576000, 192000, 6144000, 4, 3},
	{16, 24576000, 96000, 3072000, 8, 5},
	{16, 24576000, 48000, 1536000, 16, 7},
	{16, 24576000, 32000, 1024000, 24, 8},
	{16, 24576000, 24000, 768000, 32, 9},
	{16, 24576000, 16000, 512000, 48, 10},
	{16, 24576000, 12000, 384000, 64, 11},
	{16, 24576000, 8000, 256000, 96, 12},

	{32, 24576000, 192000, 12288000, 2, 2},
	{32, 24576000, 96000, 6144000, 4, 3},
	{32, 24576000, 48000, 3072000, 8, 5},
	{32, 24576000, 32000, 2048000, 12, 6},
	{32, 24576000, 24000, 1536000, 16, 7},
	{32, 24576000, 16000, 1024000, 24, 8},
	{32, 24576000, 12000, 768000, 32, 9},
	{32, 24576000, 8000, 512000, 48, 10},

	{64, 24576000, 192000, 24576000, 1, 1},
	{64, 24576000, 96000, 12288000, 2, 2},
	{64, 24576000, 48000, 6144000, 4, 3},
	{64, 24576000, 32000, 4096000, 6, 4},
	{64, 24576000, 24000, 3072000, 8, 5},
	{64, 24576000, 16000, 2048000, 12, 6},
	{64, 24576000, 12000, 1536000, 16, 7},
	{64, 24576000, 8000, 1024000, 24, 8},

	{128, 24576000, 96000, 24576000, 1, 1},
	{128, 24576000, 48000, 12288000, 2, 2},
	{128, 24576000, 24000, 6144000, 4, 3},
	{128, 24576000, 16000, 4096000, 6, 4},
	{128, 24576000, 12000, 3072000, 8, 5},
	{128, 24576000, 8000, 2048000, 12, 6},

	{256, 24576000, 48000, 24576000, 1, 1},
	{256, 24576000, 24000, 12288000, 2, 2},
	{256, 24576000, 12000, 6144000, 4, 3},
	{256, 24576000, 8000, 4096000, 6, 4},

	{512, 24576000, 24000, 24576000, 1, 1},
	{512, 24576000, 12000, 12288000, 2, 2},

	{1024, 24576000, 12000, 24576000, 1, 1},
};

static const struct _pcm0_bclk_div_s pcm_bclk_div_s[] = {
	{16, 22579200, 176400, 2822400, 8, 5},
	{16, 22579200, 88200, 1411200, 16, 7},
	{16, 22579200, 44100, 705600, 32, 9},
	{16, 22579200, 22050, 352800, 64, 11},
	{16, 22579200, 11025, 176400, 128, 13},

	{32, 22579200, 176400, 5644800, 4, 3},
	{32, 22579200, 88200, 2822400, 8, 5},
	{32, 22579200, 44100, 1411200, 16, 7},
	{32, 22579200, 22050, 705600, 32, 9},
	{32, 22579200, 11025, 352800, 64, 11},

	{64, 22579200, 176400, 11289600, 2, 2},
	{64, 22579200, 88200, 5644800, 4, 3},
	{64, 22579200, 44100, 2822400, 8, 5},
	{64, 22579200, 22050, 1411200, 16, 7},
	{64, 22579200, 11025, 705600, 32, 9},

	{128, 22579200, 176400, 22579200, 1, 1},
	{128, 22579200, 88200, 11289600, 2, 2},
	{128, 22579200, 44100, 5644800, 4, 3},
	{128, 22579200, 22050, 2822400, 8, 5},
	{128, 22579200, 11025, 1411200, 16, 7},

	{256, 22579200, 88200, 22579200, 1, 1},
	{256, 22579200, 44100, 11289600, 2, 2},
	{256, 22579200, 22050, 5644800, 4, 3},
	{256, 22579200, 11025, 2822400, 8, 5},

	{512, 22579200, 44100, 22579200, 1, 1},
	{512, 22579200, 22050, 11289600, 2, 2},
	{512, 22579200, 11025, 5644800, 4, 3},

	{1024, 22579200, 22050, 22579200, 1, 1},
	{1024, 22579200, 11025, 11289600, 2, 2},

	{16, 24576000, 192000, 3072000, 8, 5},
	{16, 24576000, 96000, 1536000, 16, 7},
	{16, 24576000, 48000, 768000, 32, 9},
	{16, 24576000, 32000, 512000, 48, 10},
	{16, 24576000, 24000, 384000, 64, 11},
	{16, 24576000, 16000, 256000, 96, 12},
	{16, 24576000, 12000, 192000, 128, 13},
	{16, 24576000, 8000, 128000, 192, 15},

	{32, 24576000, 192000, 6144000, 4, 3},
	{32, 24576000, 96000, 3072000, 8, 5},
	{32, 24576000, 48000, 1536000, 16, 7},
	{32, 24576000, 32000, 1024000, 24, 8},
	{32, 24576000, 24000, 768000, 32, 9},
	{32, 24576000, 16000, 512000, 48, 10},
	{32, 24576000, 12000, 384000, 64, 11},
	{32, 24576000, 8000, 256000, 96, 12},

	{64, 24576000, 192000, 12288000, 2, 2},
	{64, 24576000, 96000, 6144000, 4, 3},
	{64, 24576000, 48000, 3072000, 8, 5},
	{64, 24576000, 32000, 2048000, 12, 6},
	{64, 24576000, 24000, 1536000, 16, 7},
	{64, 24576000, 16000, 1024000, 24, 8},
	{64, 24576000, 12000, 768000, 32, 9},
	{64, 24576000, 8000, 512000, 48, 10},

	{128, 24576000, 192000, 24576000, 1, 1},
	{128, 24576000, 96000, 12288000, 2, 2},
	{128, 24576000, 48000, 6144000, 4, 3},
	{128, 24576000, 32000, 4096000, 6, 4},
	{128, 24576000, 24000, 3072000, 8, 5},
	{128, 24576000, 16000, 2048000, 12, 6},
	{128, 24576000, 12000, 1536000, 16, 7},
	{128, 24576000, 8000, 1024000, 24, 8},

	{256, 24576000, 96000, 24576000, 1, 1},
	{256, 24576000, 48000, 12288000, 2, 2},
	{256, 24576000, 24000, 6144000, 4, 3},
	{256, 24576000, 12000, 3072000, 8, 5},
	{256, 24576000, 8000, 2048000, 12, 6},

	{512, 24576000, 48000, 24576000, 1, 1},
	{512, 24576000, 24000, 12288000, 2, 2},
	{512, 24576000, 12000, 6144000, 4, 3},

	{1024, 24576000, 24000, 24576000, 1, 1},
	{1024, 24576000, 12000, 12288000, 2, 2},
};

void sunxi_snd_txctrl_daudio0(struct snd_pcm_substream *substream, int on,int hub_en)
{
	u32 reg_val;
	/*flush TX FIFO*/
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	reg_val |= SUNXI_DAUDIOFCTL_FTX;
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	/*clear TX counter*/
	sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIOTXCNT);

	if (on) {
		/* DAUDIO TX ENABLE */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		reg_val |= SUNXI_DAUDIOCTL_TXEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);

		/* enable DMA DRQ mode for play */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOINT);
		reg_val |= SUNXI_DAUDIOINT_TXDRQEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOINT);
	} else {
		/* DAUDIO TX DISABLE */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		reg_val &= ~SUNXI_DAUDIOCTL_TXEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		/* DISBALE dma DRQ mode */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOINT);
		reg_val &= ~SUNXI_DAUDIOINT_TXDRQEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOINT);
	}
	if (hub_en) {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val |= SUNXI_DAUDIOFCTL_HUBEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	} else {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val &= ~SUNXI_DAUDIOFCTL_HUBEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	}
}
EXPORT_SYMBOL(sunxi_snd_txctrl_daudio0);

void sunxi_snd_rxctrl_daudio0(struct snd_pcm_substream *substream, int on)
{
	int i = 0;
	u32 reg_val;
	/*flush RX FIFO*/
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	reg_val |= SUNXI_DAUDIOFCTL_FRX;	
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	/*clear RX counter*/
	sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIORXCNT);

	/*for IO TO IO transfer*/
	if (!substream) {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_TXCHCFG);
		reg_val &= ~SUNXI_TXCHCFG_RX_SLOT_NUM(CH_MAX-1);
		reg_val |= SUNXI_TXCHCFG_RX_SLOT_NUM(1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_TXCHCFG);

		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
		reg_val &= ~SUNXI_DAUDIORXCHSEL_RXOFFSET(3);
		reg_val	&= ~SUNXI_DAUDIORXCHSEL_RXCHSET(7);
		reg_val |= SUNXI_DAUDIORXCHSEL_RXOFFSET(1);
		reg_val	|= SUNXI_DAUDIORXCHSEL_RXCHSET(1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);

		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHMAP);
		reg_val = SUNXI_RXCHANMAP_DEFAULT;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIORXCHMAP);

		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val &= ~SUNXI_DAUDIOFCTL_RXOM(3);
		reg_val |= SUNXI_DAUDIOFCTL_RXOM(1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		/*clear RX counter*/
		sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIORXCNT);
	}

	if (on) {
		/* DAUDIO RX ENABLE */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		reg_val |= SUNXI_DAUDIOCTL_RXEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		/* enable DMA DRQ mode for record */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOINT);
		reg_val |= SUNXI_DAUDIOINT_RXDRQEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOINT);
	} else {
		/* DISABLE DAUDIO RX*/
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
		reg_val &= ~SUNXI_DAUDIOCTL_RXEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);

		/* DISBALE dma DRQ mode */
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOINT);
		reg_val &= ~SUNXI_DAUDIOINT_RXDRQEN;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOINT);

		/*flush RX FIFO*/
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val |= SUNXI_DAUDIOFCTL_FRX;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		/*clear RX counter*/
		sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIORXCNT);
		/*
		*	while flush RX FIFO, must read RXFIFO DATA  three times.
		*	or it wouldn't flush RX FIFO clean; and it will let record data channel reverse!
		*/
		for (i = 0; i < 9;i++) {
			reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXFIFO);
		}
	}
}
EXPORT_SYMBOL(sunxi_snd_rxctrl_daudio0);

int sunxi_daudio0_set_fmt(struct snd_soc_dai *cpu_dai, unsigned int fmt)
{
	u32 reg_val = 0;
	u32 reg_val1 = 0;
	u32 reg_val2 = 0;

	/* master or slave selection */
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	switch(fmt & SND_SOC_DAIFMT_MASTER_MASK){
		case SND_SOC_DAIFMT_CBM_CFM:   /* codec clk & frm master, ap is slave*/
			reg_val &= ~SUNXI_DAUDIOCTL_LRCKOUT;
			reg_val &= ~SUNXI_DAUDIOCTL_BCLKOUT;
			break;
		case SND_SOC_DAIFMT_CBS_CFS:   /* codec clk & frm slave,ap is master*/
			reg_val |= SUNXI_DAUDIOCTL_LRCKOUT;
			reg_val |= SUNXI_DAUDIOCTL_BCLKOUT;
			break;
		default:
			pr_err("unknwon master/slave format\n");
			return -EINVAL;
	}
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	/* pcm or daudio mode selection */
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	reg_val1 = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);
	reg_val2 = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
	reg_val &= ~SUNXI_DAUDIOCTL_MODESEL(3);
	reg_val1 &= ~SUNXI_DAUDIOTXn_OFFSET(3);
	reg_val2 &= ~(SUNXI_DAUDIORXCHSEL_RXOFFSET(3));
	switch(fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
		case SND_SOC_DAIFMT_I2S:        /* i2s mode */
			reg_val  |=  SUNXI_DAUDIOCTL_MODESEL(1);
			reg_val1 |= SUNXI_DAUDIOTXn_OFFSET(1);
			reg_val2 |= SUNXI_DAUDIORXCHSEL_RXOFFSET(1);
			break;
		case SND_SOC_DAIFMT_RIGHT_J:    /* Right Justified mode */
			reg_val  |=  SUNXI_DAUDIOCTL_MODESEL(2);
			break;
		case SND_SOC_DAIFMT_LEFT_J:     /* Left Justified mode */
			reg_val  |=  SUNXI_DAUDIOCTL_MODESEL(1);
			reg_val1 |= SUNXI_DAUDIOTXn_OFFSET(0);
			reg_val2 |= SUNXI_DAUDIORXCHSEL_RXOFFSET(0);
			break;
		case SND_SOC_DAIFMT_DSP_A:      /* L data msb after FRM LRC */
			reg_val  |=  SUNXI_DAUDIOCTL_MODESEL(0);
			reg_val1 |= SUNXI_DAUDIOTXn_OFFSET(1);
			reg_val2 |= SUNXI_DAUDIORXCHSEL_RXOFFSET(1);
			break;
		case SND_SOC_DAIFMT_DSP_B:      /* L data msb during FRM LRC */
			reg_val  |=  SUNXI_DAUDIOCTL_MODESEL(0);
			break;
		default:
			return -EINVAL;
	}
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	sunxi_smc_writel(reg_val1, sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);
	sunxi_smc_writel(reg_val2, sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
	/* DAI signal inversions */
	reg_val1 = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	switch(fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
		case SND_SOC_DAIFMT_I2S:        /* i2s mode */
		case SND_SOC_DAIFMT_RIGHT_J:    /* Right Justified mode */
		case SND_SOC_DAIFMT_LEFT_J:     /* Left Justified mode */
			switch(fmt & SND_SOC_DAIFMT_INV_MASK) {
				case SND_SOC_DAIFMT_NB_NF:     /* normal bit clock + frame */
					reg_val1 &= ~SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					reg_val1 &= ~SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_NB_IF:     /* normal bclk + inv frm */
					reg_val1 |= SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 &= ~SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_IB_NF:     /* invert bclk + nor frm */
					reg_val1 &= ~SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 |= SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_IB_IF:     /* invert bclk + frm */
					reg_val1 |= SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 |= SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				default:
					pr_warn("not support format!:%d\n", __LINE__);
					break;
			}
			break;
		case SND_SOC_DAIFMT_DSP_A:      /* L data msb after FRM LRC */
		case SND_SOC_DAIFMT_DSP_B:      /* L data msb during FRM LRC */
			switch(fmt & SND_SOC_DAIFMT_INV_MASK) {
				case SND_SOC_DAIFMT_NB_NF:     /* normal bit clock + frame */
					reg_val1 |= SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 &= ~SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_NB_IF:     /* normal bclk + inv frm */
					reg_val1 &= ~SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 &= ~SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_IB_NF:     /* invert bclk + nor frm */
					reg_val1 |= SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 |= SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				case SND_SOC_DAIFMT_IB_IF:     /* invert bclk + frm */
					reg_val1 &= ~SUNXI_DAUDIOFAT0_LRCK_POLAYITY;
					reg_val1 |= SUNXI_DAUDIOFAT0_BCLK_POLAYITY;
					break;
				default:
					pr_warn("not support format!:%d\n", __LINE__);
					break;
			}
			break;
		default:
			pr_warn("not support format!:%d\n", __LINE__);
			break;
	}
	sunxi_smc_writel(reg_val1, sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	return 0;
}
EXPORT_SYMBOL(sunxi_daudio0_set_fmt);

int sunxi_daudio0_hw_params(struct snd_pcm_substream *substream,
																struct snd_pcm_hw_params *params,
																struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sunxi_dma_params *dma_data;
	u32 reg_val = 0;

	switch (params_format(params))
	{
		case SNDRV_PCM_FORMAT_S16_LE:
			sunxi_daudio.samp_res = 16;
			break;
		case SNDRV_PCM_FORMAT_S20_3LE:
			sunxi_daudio.samp_res = 24;
			break;
		case SNDRV_PCM_FORMAT_S24_LE:
			sunxi_daudio.samp_res = 24;
			break;
		case SNDRV_PCM_FORMAT_S32_LE:
			sunxi_daudio.samp_res = 24;
			break;
		default:
			return -EINVAL;
	}
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	reg_val &= ~SUNXI_DAUDIOFAT0_SR(7);
	if(sunxi_daudio.samp_res == 16)
		reg_val |= SUNXI_DAUDIOFAT0_SR(3);
	else if(sunxi_daudio.samp_res == 20)
		reg_val |= SUNXI_DAUDIOFAT0_SR(4);
	else
		reg_val |= SUNXI_DAUDIOFAT0_SR(5);
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFAT0);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val &= ~SUNXI_DAUDIOFCTL_TXIM;
		if (sunxi_daudio.samp_res == 24) {
			reg_val &= ~SUNXI_DAUDIOFCTL_TXIM;
		} else {
			reg_val |= SUNXI_DAUDIOFCTL_TXIM;
		}
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	} else {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
		reg_val &= ~SUNXI_DAUDIOFCTL_RXOM(3);
		if (sunxi_daudio.samp_res == 24) {
			reg_val &= ~SUNXI_DAUDIOFCTL_RXOM(3);
		} else {
			reg_val |= SUNXI_DAUDIOFCTL_RXOM(1);
		}
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	}

	/* play or record */
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &sunxi_daudio_pcm_stereo_out;
	else
		dma_data = &sunxi_daudio_pcm_stereo_in;

	snd_soc_dai_set_dma_data(rtd->cpu_dai, substream, dma_data);
	return 0;
}
EXPORT_SYMBOL(sunxi_daudio0_hw_params);

int r_i2s_tx_disable(void)
{
	int reg_val;
	/* DAUDIO TX DISABLE */
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	reg_val &= ~SUNXI_DAUDIOCTL_TXEN;
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	return 0;
}
EXPORT_SYMBOL(r_i2s_tx_disable);

int r_i2s_rx_disable(void)
{
	int reg_val;
	/* DAUDIO RX DISABLE */
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	reg_val &= ~SUNXI_DAUDIOCTL_RXEN;
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	return 0;
}
EXPORT_SYMBOL(r_i2s_rx_disable);

static int sunxi_daudio0_trigger(struct snd_pcm_substream *substream,
                              int cmd, struct snd_soc_dai *dai)
{
	int ret = 0;
	u32 reg_val;
	switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_RESUME:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
				sunxi_snd_rxctrl_daudio0(substream, 1);
			} else {
				sunxi_snd_txctrl_daudio0(substream, 1,0);
			}
			reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
			if (daudio0_loop_en) {
				reg_val |= SUNXI_DAUDIOCTL_LOOP; /*for test*/
			}
			sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
			break;
		case SNDRV_PCM_TRIGGER_STOP:
		case SNDRV_PCM_TRIGGER_SUSPEND:
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
				sunxi_snd_rxctrl_daudio0(substream, 0);
			} else {
			  	sunxi_snd_txctrl_daudio0(substream, 0,0);
			}
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

int sunxi_daudio0_set_sysclk(struct snd_soc_dai *cpu_dai, int clk_id,
                                 unsigned int freq, int daudio_pcm_select)
{
#ifdef CONFIG_ARCH_SUN9IW1
	if (clk_set_rate(daudio_pll3clk, freq)) {
		pr_err("try to set the daudio_pll2clk failed!\n");
	}
	/*
	*22579200:reg:0xf6000008 val is 0x8104361c
	*	  reg:0x06000108 val is 0xc00121ff
	*24576000:reg:0xf6000008 val is 0x81043d1d
	*	  reg:0x06000108 val is 0xc000e147
	*/
	pr_debug("0xf6000008 = %#x, line:%d, freq:%d\n", sunxi_smc_readl((void __iomem *)0xf6000008), __LINE__, freq);
#else
	#ifdef CONFIG_ARCH_SUN8IW7
		if (clk_set_rate(daudio_pll, freq)) {
			pr_err("try to set the daudio_pll rate failed!\n");
		}
	#else
	if (clk_set_rate(daudio_pll2clk, freq)) {
		pr_err("try to set the daudio_pll2clk failed!\n");
	}
	#endif
#endif
	sunxi_daudio.daudio_select = daudio_pcm_select;

	return 0;
}
EXPORT_SYMBOL(sunxi_daudio0_set_sysclk);

int sunxi_daudio0_set_rate(int freq) {
#ifdef CONFIG_ARCH_SUN9IW1
	if (clk_set_rate(daudio_pll3clk, freq)) {
			pr_err("try to set the i2s_pll3clk failed!\n");
	}
#else
	#ifdef CONFIG_ARCH_SUN8IW7
		if (clk_set_rate(daudio_pll, freq)) {
			pr_err("try to set the daudio_pll rate failed!\n");
		}
	#else
	if (clk_set_rate(daudio_pll2clk, freq)) {
		pr_err("try to set the daudio_pll2clk failed!\n");
	}
	#endif
#endif
	return 0;
}

EXPORT_SYMBOL(sunxi_daudio0_set_rate);

int sunxi_daudio0_set_clkdiv(struct snd_soc_dai *cpu_dai, int div_id, int sample_rate)
{
	int i;
	u32 reg_val = 0;
	u32 mclk_div = 1;
	u32 bclk_div_regval = 1;

	/*enable mclk*/
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCLKD);
	reg_val &= ~(SUNXI_DAUDIOCLKD_MCLKOEN);
	reg_val |= SUNXI_DAUDIOCLKD_MCLKOEN;
	reg_val &= ~SUNXI_DAUDIOCLKD_MCLKDIV(0xf);
	reg_val &= ~SUNXI_DAUDIOCLKD_BCLKDIV(0xf);
	reg_val |= SUNXI_DAUDIOCLKD_MCLKDIV(mclk_div)<<0;
	/*i2s mode*/
	if (sunxi_daudio.daudio_select) {
		for (i = 0; i < ARRAY_SIZE(i2s_bclk_div_s); i++) {
			if ((i2s_bclk_div_s[i].i2s_lrck_period == sunxi_daudio.pcm_lrck_period)\
					&&(i2s_bclk_div_s[i].sample_rate == sample_rate)) {
				bclk_div_regval = i2s_bclk_div_s[i].bclk_div_regval;
				break;
			}
		}
	} else {
		/*pcm mode*/
		for (i = 0; i < ARRAY_SIZE(pcm_bclk_div_s); i++) {
			if ((pcm_bclk_div_s[i].pcm_lrck_period == sunxi_daudio.pcm_lrck_period)\
					&&(pcm_bclk_div_s[i].sample_rate == sample_rate)) {
				bclk_div_regval = pcm_bclk_div_s[i].bclk_div_regval;
				break;
			}
		}
	}
	reg_val |= SUNXI_DAUDIOCLKD_BCLKDIV(bclk_div_regval);
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCLKD);

	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	reg_val &= ~SUNXI_DAUDIOFAT0_LRCKR_PERIOD(0x3ff);
	reg_val &= ~SUNXI_DAUDIOFAT0_LRCK_PERIOD(0x3ff);
	reg_val |= SUNXI_DAUDIOFAT0_LRCK_PERIOD(sunxi_daudio.pcm_lrck_period-1);
	reg_val |= SUNXI_DAUDIOFAT0_LRCKR_PERIOD(sunxi_daudio.pcm_lrckr_period-1);
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFAT0);

	/* slot size select */
	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	reg_val &= ~SUNXI_DAUDIOFAT0_SW(7);
	if(sunxi_daudio.slot_width == 16)
		reg_val |= SUNXI_DAUDIOFAT0_SW(3);
	else if(sunxi_daudio.slot_width == 20) 
		reg_val |= SUNXI_DAUDIOFAT0_SW(4);
	else if(sunxi_daudio.slot_width == 24)
		reg_val |= SUNXI_DAUDIOFAT0_SW(5);
	else if(sunxi_daudio.slot_width == 32)
		reg_val |= SUNXI_DAUDIOFAT0_SW(7);
	else
		reg_val |= SUNXI_DAUDIOFAT0_SW(1);

	reg_val &= ~SUNXI_DAUDIOFAT0_SR(7);
	if(sunxi_daudio.samp_res == 16)
		reg_val |= SUNXI_DAUDIOFAT0_SR(3);
	else if(sunxi_daudio.samp_res == 20)
		reg_val |= SUNXI_DAUDIOFAT0_SR(4);
	else
		reg_val |= SUNXI_DAUDIOFAT0_SR(5);
	if(sunxi_daudio.frametype)
		reg_val |= SUNXI_DAUDIOFAT0_LRCK_WIDTH;	
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFAT0);

	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT1);
	if (sunxi_daudio.msb_lsb_first) {
		reg_val |= SUNXI_DAUDIOFAT1_TX_MLS;
		reg_val |= SUNXI_DAUDIOFAT1_RX_MLS;
	} else {
		reg_val &= ~SUNXI_DAUDIOFAT1_TX_MLS;
		reg_val &= ~SUNXI_DAUDIOFAT1_RX_MLS;
	}
	reg_val &= ~SUNXI_DAUDIOFAT1_SEXT(3);
	reg_val |= SUNXI_DAUDIOFAT1_SEXT(sunxi_daudio.signext);

	/*linear or u/a-law*/
	reg_val &= ~SUNXI_DAUDIOFAT1_RX_PDM(3);
	reg_val &= ~SUNXI_DAUDIOFAT1_TX_PDM(3);
	reg_val |= SUNXI_DAUDIOFAT1_TX_PDM(sunxi_daudio.tx_data_mode);
	reg_val |= SUNXI_DAUDIOFAT1_RX_PDM(sunxi_daudio.rx_data_mode);
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOFAT1);

	return 0;
}
EXPORT_SYMBOL(sunxi_daudio0_set_clkdiv);

int sunxi_daudio0_perpare(struct snd_pcm_substream *substream,
	struct snd_soc_dai *cpu_dai)
{
	u32 reg_val;

	/*i2s mode*/
	if (sunxi_daudio.daudio_select) {
		if ((sunxi_daudio.slot_width * substream->runtime->channels) > 2*sunxi_daudio.pcm_lrck_period) {
			pr_warning("not support slot_width,pcm_lrck_period. slot_width*channel should <= 2*sunxi_daudio.pcm_lrck_period!\n");
		}
	} else {
		/*pcm mode*/
		if ((sunxi_daudio.slot_width * substream->runtime->channels) > sunxi_daudio.pcm_lrck_period) {
			pr_warning("not support slot_width,pcm_lrck_period. slot_width*channel should <= sunxi_daudio.pcm_lrck_period!\n");
		}
	}
	/* play or record */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_TXCHCFG);
		reg_val &= ~SUNXI_TXCHCFG_TX_SLOT_NUM(CH_MAX-1);
		reg_val |= SUNXI_TXCHCFG_TX_SLOT_NUM(substream->runtime->channels-1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_TXCHCFG);

		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);
		reg_val &= ~SUNXI_DAUDIOTXn_CHEN(CHEN_MASK);
		reg_val &= ~SUNXI_DAUDIOTXn_CHSEL(CHSEL_MASK);
		reg_val |= SUNXI_DAUDIOTXn_CHEN((CHEN_MASK>>(CH_MAX-substream->runtime->channels)));
		reg_val |= SUNXI_DAUDIOTXn_CHSEL(substream->runtime->channels-1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);

		reg_val = SUNXI_TXCHANMAP_DEFAULT;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOTX0CHMAP);

		/*clear TX counter*/
		sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIOTXCNT);
	} else {
		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_TXCHCFG);
		reg_val &= ~SUNXI_TXCHCFG_RX_SLOT_NUM(CH_MAX-1);
		reg_val |= SUNXI_TXCHCFG_RX_SLOT_NUM(substream->runtime->channels-1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_TXCHCFG);

		reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
		reg_val &= ~SUNXI_DAUDIORXCHSEL_RXCHSET(CHSEL_MASK);
		reg_val |= SUNXI_DAUDIORXCHSEL_RXCHSET(substream->runtime->channels-1);
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);

		reg_val = SUNXI_RXCHANMAP_DEFAULT;
		sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIORXCHMAP);

		/*clear RX counter*/
		sunxi_smc_writel(0, sunxi_daudio.regs + SUNXI_DAUDIORXCNT);
	}
	return 0;
}
EXPORT_SYMBOL(sunxi_daudio0_perpare);

static int sunxi_daudio0_dai_probe(struct snd_soc_dai *dai)
{
	return 0;
}

static int sunxi_daudio0_dai_remove(struct snd_soc_dai *dai)
{
	return 0;
}

static void daudioregsave(void)
{
	regsave[0] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	regsave[1] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	regsave[2] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFAT1);
	regsave[3] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOFCTL) | SUNXI_DAUDIOFCTL_FRX |SUNXI_DAUDIOFCTL_FTX;
	regsave[4] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOINT);
	regsave[5] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCLKD);
	regsave[6] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_TXCHCFG);
	regsave[7] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);
	regsave[8] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOTX0CHMAP);
	regsave[9] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
	regsave[10] = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIORXCHMAP);
}

static void daudioregrestore(void)
{
	sunxi_smc_writel(regsave[0], sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	sunxi_smc_writel(regsave[1], sunxi_daudio.regs + SUNXI_DAUDIOFAT0);
	sunxi_smc_writel(regsave[2], sunxi_daudio.regs + SUNXI_DAUDIOFAT1);
	sunxi_smc_writel(regsave[3], sunxi_daudio.regs + SUNXI_DAUDIOFCTL);
	sunxi_smc_writel(regsave[4], sunxi_daudio.regs + SUNXI_DAUDIOINT);
	sunxi_smc_writel(regsave[5], sunxi_daudio.regs + SUNXI_DAUDIOCLKD);

	sunxi_smc_writel(regsave[6], sunxi_daudio.regs + SUNXI_TXCHCFG);
	sunxi_smc_writel(regsave[7], sunxi_daudio.regs + SUNXI_DAUDIOTX0CHSEL);
	sunxi_smc_writel(regsave[8], sunxi_daudio.regs + SUNXI_DAUDIOTX0CHMAP);
	sunxi_smc_writel(regsave[9], sunxi_daudio.regs + SUNXI_DAUDIORXCHSEL);
	sunxi_smc_writel(regsave[10], sunxi_daudio.regs + SUNXI_DAUDIORXCHMAP);
}

static int daudio0_global_enable(bool on)
{
	u32 reg_val = 0;
	u32 reg_val1 = 0;

	reg_val = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	reg_val1 = sunxi_smc_readl(sunxi_daudio.regs + SUNXI_DAUDIOCLKD);
	if (!on) {
		reg_val &= ~SUNXI_DAUDIOCTL_GEN;
		reg_val &= ~SUNXI_DAUDIOCTL_SDO0EN;
		reg_val1 &= ~SUNXI_DAUDIOCLKD_MCLKOEN;
	} else {
		reg_val |= SUNXI_DAUDIOCTL_GEN;
		reg_val |= SUNXI_DAUDIOCTL_SDO0EN;
		reg_val1 |= SUNXI_DAUDIOCLKD_MCLKOEN;
		reg_val1 |= SUNXI_DAUDIOCLKD_MCLKDIV(1);
	}
	sunxi_smc_writel(reg_val, sunxi_daudio.regs + SUNXI_DAUDIOCTL);
	sunxi_smc_writel(reg_val1, sunxi_daudio.regs + SUNXI_DAUDIOCLKD);

	return 0;
}

static int sunxi_daudio0_suspend(struct snd_soc_dai *cpu_dai)
{
	u32 pin_index = 0;
	u32 config;
	struct gpio_config *pin_daudio0_cfg;
	char pin_name[SUNXI_PIN_NAME_MAX_LEN];
	pr_debug("[DAUDIO]Entered %s\n", __func__);

	daudio0_global_enable(0);
	daudioregsave();

	if ((NULL == daudio_moduleclk) ||(IS_ERR(daudio_moduleclk))) {
		pr_err("daudio_moduleclk handle is invalid, just return\n");
		return -EFAULT;
	} else {
		/*release the module clock*/
		clk_disable(daudio_moduleclk);
	}
	#if defined(CONFIG_ARCH_SUN8IW6) || defined(CONFIG_ARCH_SUN8IW8)
	if ((NULL == daudio_pll2clk) ||(IS_ERR(daudio_pll2clk))) {
		pr_err("daudio_pll2clk handle is invalid, just return\n");
		return -EFAULT;
	} else {
		/*release the module clock*/
		clk_disable(daudio_pll2clk);
	}
	if ((NULL == daudio_pllx8) ||(IS_ERR(daudio_pllx8))) {
		pr_err("daudio_pllx8 handle is invalid, just return\n");
		return -EFAULT;
	} else {
		/*release the module clock*/
		clk_disable(daudio_pllx8);
	}
	#endif
	devm_pinctrl_put(daudio_pinctrl);
	/* request pin individually */
	for (pin_index = 0; pin_index < pin_count; pin_index++) {
		pin_daudio0_cfg = &(pin_daudio0_list[pin_index].gpio);
		/* valid pin of sunxi-pinctrl, config pin attributes individually.*/
		sunxi_gpio_to_name(pin_daudio0_cfg->gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
	}
	return 0;
}

static int sunxi_daudio0_resume(struct snd_soc_dai *cpu_dai)
{
	pr_debug("[DAUDIO]Entered %s\n", __func__);
	#if defined(CONFIG_ARCH_SUN8IW6) || defined(CONFIG_ARCH_SUN8IW8)
	/*enable the module clock*/
	if (clk_prepare_enable(daudio_pllx8)) {
		pr_err("open daudio_pllx8 failed! line = %d\n", __LINE__);
	}
	/*enable the module clock*/
	if (clk_prepare_enable(daudio_pll2clk)) {
		pr_err("open daudio_pll2clk failed! line = %d\n", __LINE__);
	}
	#endif
	/*enable the module clock*/
	if (clk_prepare_enable(daudio_moduleclk)) {
		pr_err("open daudio_moduleclk failed! line = %d\n", __LINE__);
	}
	/*for evb pin config*/
	daudio_pinctrl = devm_pinctrl_get_select_default(cpu_dai->dev);
	if (IS_ERR_OR_NULL(daudio_pinctrl)) {
		dev_warn(cpu_dai->dev,
			"pins are not configured from the driver\n");
	}

	daudioregrestore();
	daudio0_global_enable(1);
	return 0;
}

#define SUNXI_DAUDIO_RATES (SNDRV_PCM_RATE_8000_192000 | SNDRV_PCM_RATE_KNOT)
static struct snd_soc_dai_ops sunxi_daudio_dai_ops = {
	.trigger 	= sunxi_daudio0_trigger,
	.hw_params 	= sunxi_daudio0_hw_params,
	.set_fmt 	= sunxi_daudio0_set_fmt,
	.set_clkdiv = sunxi_daudio0_set_clkdiv,
	.set_sysclk = sunxi_daudio0_set_sysclk,
	.prepare  =	sunxi_daudio0_perpare,
};

static struct snd_soc_dai_driver sunxi_daudio_dai[] = {
	{
		.name = "pri_dai",
		.id = 1,
	.probe 		= sunxi_daudio0_dai_probe,
	.suspend 	= sunxi_daudio0_suspend,
	.resume 	= sunxi_daudio0_resume,
	.remove 	= sunxi_daudio0_dai_remove,
	.playback 	= {
		.channels_min = 1,
		.channels_max = 8,
		.rates = SUNXI_DAUDIO_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
	},
	.capture 	= {
		.channels_min = 1,
		.channels_max = 8,
		.rates = SUNXI_DAUDIO_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
	},
	.ops 		= &sunxi_daudio_dai_ops,	
	},
	{
		.name           = "sec_dai",
		.id = 2,
		.playback 	= {
			.channels_min = 1,
			.channels_max = 8,
			.rates = SUNXI_DAUDIO_RATES,
			.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
		},
		.capture 	= {
			.channels_min = 1,
			.channels_max = 8,
			.rates = SUNXI_DAUDIO_RATES,
			.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
		},
		.ops 		= &sunxi_daudio_dai_ops,

	}
};

static int __init sunxi_daudio0_dev_probe(struct platform_device *pdev)
{
	int ret = 0;
	script_item_u val;
	script_item_value_type_e  type;

	sunxi_daudio.regs = ioremap(SUNXI_DAUDIOBASE, 0x100);
	if (sunxi_daudio.regs == NULL) {
		return -ENXIO;
	}
	/*for pin config*/
	daudio_pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR_OR_NULL(daudio_pinctrl)) {
		dev_warn(&pdev->dev,
			"pins are not configured from the driver\n");
	}
	pin_count = script_get_pio_list (TDM_NAME, &pin_daudio0_list);
	if (pin_count == 0) {
		/* "daudio0" have no pin configuration */
		pr_err("daudio0 have no pin configuration\n");
	}

#ifdef CONFIG_ARCH_SUN9IW1
	/*daudio pll2clk*/
	daudio_pll3clk = clk_get(NULL, "pll3");
	if ((!daudio_pll3clk)||(IS_ERR(daudio_pll3clk))) {
		pr_err("try to get daudio_pll2clk failed\n");
	}
	if (clk_prepare_enable(daudio_pll3clk)) {
		pr_err("enable daudio_pll2clk failed; \n");
	}
	/*daudio module clk*/
	daudio_moduleclk = clk_get(NULL, "cpuri2s1");
	if ((!daudio_moduleclk)||(IS_ERR(daudio_moduleclk))) {
		pr_err("try to get daudio_moduleclk failed\n");
	}
	if (clk_set_parent(daudio_moduleclk, daudio_pll3clk)) {
		pr_err("try to set parent of daudio_moduleclk to daudio_pll2ck failed! line = %d\n",__LINE__);
	}
	if (clk_set_rate(daudio_pll3clk, 24576000)) {
		pr_err("set daudio_moduleclk clock freq to 24576000 failed! line = %d\n", __LINE__);
	}

	if (clk_prepare_enable(daudio_moduleclk)) {
		pr_err("open daudio_moduleclk failed! line = %d\n", __LINE__);
	}
#else
#ifdef CONFIG_ARCH_SUN8IW7
	daudio_pll = clk_get(NULL, "pll_audio");
	if ((!daudio_pll)||(IS_ERR(daudio_pll))) {
		pr_err("try to get daudio_pll failed\n");
	}
	if (clk_prepare_enable(daudio_pll)) {
		pr_err("enable daudio_pll2clk failed; \n");
	}
	/*daudio module clk*/
	daudio_moduleclk = clk_get(NULL, "i2s0");
	if ((!daudio_moduleclk)||(IS_ERR(daudio_moduleclk))) {
		pr_err("try to get daudio_moduleclk failed\n");
	}
	if (clk_set_parent(daudio_moduleclk, daudio_pll)) {
		pr_err("try to set parent of daudio_moduleclk to daudio_pll2ck failed! line = %d\n",__LINE__);
	}

	if (clk_set_rate(daudio_moduleclk, 24576000)) {
		pr_err("set daudio_moduleclk clock freq to 24576000 failed! line = %d\n", __LINE__);
	}

	if (clk_prepare_enable(daudio_moduleclk)) {
		pr_err("open daudio_moduleclk failed! line = %d\n", __LINE__);
	}
#else
	daudio_pllx8 = clk_get(NULL, "pll_audiox8");
	if ((!daudio_pllx8)||(IS_ERR(daudio_pllx8))) {
		pr_err("try to get daudio_pllx8 failed\n");
	}
	if (clk_prepare_enable(daudio_pllx8)) {
		pr_err("enable daudio_pll2clk failed; \n");
	}

	/*daudio pll2clk*/
	daudio_pll2clk = clk_get(NULL, "pll2");
	if ((!daudio_pll2clk)||(IS_ERR(daudio_pll2clk))) {
		pr_err("try to get daudio_pll2clk failed\n");
	}
	if (clk_prepare_enable(daudio_pll2clk)) {
		pr_err("enable daudio_pll2clk failed; \n");
	}
#ifdef CONFIG_ARCH_SUN8IW8
	/*daudio module clk*/
	daudio_moduleclk = clk_get(NULL, "i2s0");
#else
	daudio_moduleclk = clk_get(NULL, "tdm");
#endif
	if ((!daudio_moduleclk)||(IS_ERR(daudio_moduleclk))) {
		pr_err("try to get daudio_moduleclk failed\n");
	}

	if (clk_set_parent(daudio_moduleclk, daudio_pll2clk)) {
		pr_err("try to set parent of daudio_moduleclk to daudio_pll2ck failed! line = %d\n",__LINE__);
	}
#if defined(CONFIG_ARCH_SUN8IW6) || defined(CONFIG_ARCH_SUN8IW8)
	if (clk_set_rate(daudio_moduleclk, 24576000)) {
		pr_err("set daudio_moduleclk clock freq to 24576000 failed! line = %d\n", __LINE__);
	}
#else
	if (clk_set_rate(daudio_moduleclk, 24576000/8)) {
		pr_err("set daudio_moduleclk clock freq to 24576000 failed! line = %d\n", __LINE__);
	}
#endif
	if (clk_prepare_enable(daudio_moduleclk)) {
		pr_err("open daudio_moduleclk failed! line = %d\n", __LINE__);
	}
#endif
#endif
	daudio0_global_enable(1);

	type = script_get_item(TDM_NAME, "sample_resolution", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] sample_resolution type err!\n");
    }
	sunxi_daudio.samp_res = val.val;

	type = script_get_item(TDM_NAME, "slot_width_select", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] slot_width_select type err!\n");
    }
	sunxi_daudio.slot_width = val.val;

	type = script_get_item(TDM_NAME, "pcm_lrck_period", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] pcm_lrck_period type err!\n");
    }
	sunxi_daudio.pcm_lrck_period = val.val;

	type = script_get_item(TDM_NAME, "pcm_lrckr_period", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] pcm_lrckr_period type err!\n");
    }
	sunxi_daudio.pcm_lrckr_period = val.val;

	type = script_get_item(TDM_NAME, "msb_lsb_first", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] msb_lsb_first type err!\n");
    }
	sunxi_daudio.msb_lsb_first = val.val;

	type = script_get_item(TDM_NAME, "sign_extend", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] sign_extend type err!\n");
    }
	sunxi_daudio.sign_extend = val.val;

	type = script_get_item(TDM_NAME, "frame_width", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] frame_width type err!\n");
    }
	sunxi_daudio.frame_width = val.val;
	type = script_get_item(TDM_NAME, "tx_data_mode", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] tx_data_mode type err!\n");
    }
	sunxi_daudio.tx_data_mode = val.val;

	type = script_get_item(TDM_NAME, "rx_data_mode", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] rx_data_mode type err!\n");
    }
	sunxi_daudio.rx_data_mode = val.val;

	ret = snd_soc_register_dais(&pdev->dev,sunxi_daudio_dai, ARRAY_SIZE(sunxi_daudio_dai));
	if (ret) {
		dev_err(&pdev->dev, "Failed to register DAI\n");
	}

	return 0;
}

static int __exit sunxi_daudio0_dev_remove(struct platform_device *pdev)
{
	if (sunxi_daudio.daudio_used) {
		sunxi_daudio.daudio_used = 0;

		if ((NULL == daudio_moduleclk) ||(IS_ERR(daudio_moduleclk))) {
			pr_err("daudio_moduleclk handle is invalid, just return\n");
			return -EFAULT;
		} else {
			/*release the module clock*/
			clk_disable(daudio_moduleclk);
		}
		#ifdef CONFIG_ARCH_SUN8IW7
		if ((NULL == daudio_pll) ||(IS_ERR(daudio_pll))) {
			pr_err("daudio_pll handle is invalid, just return\n");
			return -EFAULT;
		} else {
			/*reease pll clk*/
			clk_put(daudio_pll);
		}
		#else
		#ifndef CONFIG_ARCH_SUN9I
		if ((NULL == daudio_pllx8) ||(IS_ERR(daudio_pllx8))) {
			pr_err("daudio_pllx8 handle is invalid, just return\n");
			return -EFAULT;
		} else {
			/*reease pllx8clk*/
			clk_put(daudio_pllx8);
		}
		#endif
		#endif
		#ifdef CONFIG_ARCH_SUN9I
		if ((NULL == daudio_pll3clk) ||(IS_ERR(daudio_pll3clk))) {
			pr_err("daudio_pll2clk handle is invalid, just return\n");
			return -EFAULT;
		} else {
			clk_put(daudio_pll3clk);
		}
		#else
		#ifndef CONFIG_ARCH_SUN8IW7
		if ((NULL == daudio_pll2clk) ||(IS_ERR(daudio_pll2clk))) {
			pr_err("daudio_pll2clk handle is invalid, just return\n");
			return -EFAULT;
		} else {
			/*release pll2clk*/
			clk_put(daudio_pll2clk);
		}
		#endif
		#endif

		devm_pinctrl_put(daudio_pinctrl);
		snd_soc_unregister_dai(&pdev->dev);
		platform_set_drvdata(pdev, NULL);
	}
	return 0;
}

/*data relating*/
static struct platform_device sunxi_daudio_device = {
	#ifdef CONFIG_ARCH_SUN9I
	.name 	= "s_i2s1",
	#elif defined CONFIG_ARCH_SUN8IW7
	.name 	= "pcm0",
	#else
	.name 	= "tdm0",
	#endif
	.id 	= PLATFORM_DEVID_NONE,
};

/*method relating*/
static struct platform_driver sunxi_daudio_driver = {
	.probe = sunxi_daudio0_dev_probe,
	.remove = __exit_p(sunxi_daudio0_dev_remove),
	.driver = {
		#ifdef CONFIG_ARCH_SUN9I
		.name = "s_i2s1",
		#elif defined CONFIG_ARCH_SUN8IW7
		.name 	= "pcm0",
		#else
		.name = "tdm0",
		#endif
		.owner = THIS_MODULE,
	},
};

static int __init sunxi_daudio0_init(void)
{	
	int err = 0;
	script_item_u val;
	script_item_value_type_e  type;

	type = script_get_item(TDM_NAME, "daudio_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] type err!\n");
    }

	sunxi_daudio.daudio_used = val.val;

	type = script_get_item(TDM_NAME, "daudio_select", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        pr_err("[DAUDIO] daudio_select:%s,line:%d type err!\n", __func__, __LINE__);
    }
	sunxi_daudio.daudio_select = val.val;

	if (sunxi_daudio.daudio_used) {
		if((err = platform_device_register(&sunxi_daudio_device)) < 0)
			return err;
	
		if ((err = platform_driver_register(&sunxi_daudio_driver)) < 0)
			return err;	
	} else {
        pr_err("[DAUDIO]sunxi-daudio cannot find any using configuration for controllers, return directly!\n");
        return 0;
    }

	return 0;
}
module_init(sunxi_daudio0_init);
module_param_named(daudio0_loop_en, daudio0_loop_en, bool, S_IRUGO | S_IWUSR);

static void __exit sunxi_daudio0_exit(void)
{
	platform_driver_unregister(&sunxi_daudio_driver);
}
module_exit(sunxi_daudio0_exit);

/* Module information */
MODULE_AUTHOR("huangxin");
MODULE_DESCRIPTION("sunxi DAUDIO SoC Interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sunxi-daudio");

