/*
 * sound\soc\sunxi\daudio\sunxi-daudio.h
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

#ifndef SUNXI_DAUDIO_H_
#define SUNXI_DAUDIO_H_

#ifdef CONFIG_ARCH_SUN8IW7
#define TDM_NAME "pcm1"
#endif

/*------------------------------------------------------------*/
/* REGISTER definition */

#ifdef CONFIG_ARCH_SUN8IW7
#define SUNXI_DAUDIO1BASE 							(0x01c22400)
#endif
	#define SUNXI_DAUDIOCTL 	  					(0x00)
	#define SUNXI_DAUDIOCTL_BCLKOUT					(1<<18)
	#define SUNXI_DAUDIOCTL_LRCKOUT					(1<<17)
	#define SUNXI_DAUDIOCTL_LRCKROUT				(1<<16)
	#define SUNXI_DAUDIOCTL_SDO3EN					(1<<11)
	#define SUNXI_DAUDIOCTL_SDO2EN					(1<<10)
	#define SUNXI_DAUDIOCTL_SDO1EN					(1<<9)
	#define SUNXI_DAUDIOCTL_SDO0EN					(1<<8) 
	#define SUNXI_DAUDIOCTL_OUTMUTE					(1<<6)
	#define SUNXI_DAUDIOCTL_MODESEL(v)				((v)<<4)
	#define SUNXI_DAUDIOCTL_LOOP					(1<<3)
	#define SUNXI_DAUDIOCTL_TXEN					(1<<2)
	#define SUNXI_DAUDIOCTL_RXEN					(1<<1)
	#define SUNXI_DAUDIOCTL_GEN						(1<<0)

#define SUNXI_DAUDIOFAT0 							(0x04)
	#define SUNXI_DAUDIOFAT0_SDI_SYNC_SEL			(1<<31)
	#define SUNXI_DAUDIOFAT0_LRCK_WIDTH				(1<<30)
	#define SUNXI_DAUDIOFAT0_LRCKR_PERIOD(v)		((v)<<20)
	#define SUNXI_DAUDIOFAT0_LRCK_POLAYITY			(1<<19)
	#define SUNXI_DAUDIOFAT0_LRCK_PERIOD(v)			((v)<<8)
	#define SUNXI_DAUDIOFAT0_BCLK_POLAYITY			(1<<7)
	#define SUNXI_DAUDIOFAT0_SR(v)					((v)<<4)
	#define SUNXI_DAUDIOFAT0_EDGE_TRANSFER			(1<<3)
	#define SUNXI_DAUDIOFAT0_SW(v)					((v)<<0)

#define SUNXI_DAUDIOFAT1							(0x08)
	#define SUNXI_DAUDIOFAT1_RX_MLS					(1<<7)
	#define SUNXI_DAUDIOFAT1_TX_MLS					(1<<6)
	#define SUNXI_DAUDIOFAT1_SEXT(v)				((v)<<4)
	#define SUNXI_DAUDIOFAT1_RX_PDM(v)				((v)<<2)
	#define SUNXI_DAUDIOFAT1_TX_PDM(v)				((v)<<0)

#define SUNXI_DAUDIOISTA 							(0x0c)
	#define SUNXI_DAUDIOSTA_TXU_INT					(1<<6)
	#define SUNXI_DAUDIOSTA_TXO_INT					(1<<5)
	#define SUNXI_DAUDIOSTA_TXE_INT					(1<<4)
	#define SUNXI_DAUDIOSTA_RXU_INT					(1<<2)
	#define SUNXI_DAUDIOSTA_RXO_INT					(1<<1)
	#define SUNXI_DAUDIOSTA_RXA_INT					(1<<0)

#define SUNXI_DAUDIORXFIFO							(0x10)

#define SUNXI_DAUDIOFCTL							(0x14)
	#define SUNXI_DAUDIOFCTL_HUBEN					(1<<31)
	#define SUNXI_DAUDIOFCTL_FTX					(1<<25)
	#define SUNXI_DAUDIOFCTL_FRX					(1<<24)
	#define SUNXI_DAUDIOFCTL_TXTL(v)				((v)<<12)
	#define SUNXI_DAUDIOFCTL_RXTL(v)				((v)<<4)
	#define SUNXI_DAUDIOFCTL_TXIM					(1<<2)
	#define SUNXI_DAUDIOFCTL_RXOM(v)				((v)<<0)

#define SUNXI_DAUDIOFSTA   							(0x18)
	#define SUNXI_DAUDIOFSTA_TXE					(1<<28)
	#define SUNXI_DAUDIOFSTA_TXECNT(v)				((v)<<16)
	#define SUNXI_DAUDIOFSTA_RXA					(1<<8)
	#define SUNXI_DAUDIOFSTA_RXACNT(v)				((v)<<0)
	
#define SUNXI_DAUDIOINT    							(0x1c)
	#define SUNXI_DAUDIOINT_TXDRQEN					(1<<7)
	#define SUNXI_DAUDIOINT_TXUIEN					(1<<6)
	#define SUNXI_DAUDIOINT_TXOIEN					(1<<5)
	#define SUNXI_DAUDIOINT_TXEIEN					(1<<4)
	#define SUNXI_DAUDIOINT_RXDRQEN					(1<<3)
	#define SUNXI_DAUDIOINT_RXUIEN					(1<<2)
	#define SUNXI_DAUDIOINT_RXOIEN					(1<<1)
	#define SUNXI_DAUDIOINT_RXAIEN					(1<<0)

#define SUNXI_DAUDIOTXFIFO							(0x20)
	
#define SUNXI_DAUDIOCLKD   							(0x24)
	#define SUNXI_DAUDIOCLKD_MCLKOEN				(1<<8)
	#define SUNXI_DAUDIOCLKD_BCLKDIV(v)				((v)<<4)
	#define SUNXI_DAUDIOCLKD_MCLKDIV(v)				((v)<<0)

#define SUNXI_DAUDIOTXCNT  						(0x28)

#define SUNXI_DAUDIORXCNT  						(0x2C)

#define SUNXI_TXCHCFG							(0x30)
	#define SUNXI_TXCHCFG_TX_SLOT_HIZ			(1<<9)
	#define SUNXI_TXCHCFG_TX_STATE				(1<<8)
	#define SUNXI_TXCHCFG_RX_SLOT_NUM(v)		((v)<<4)
	#define SUNXI_TXCHCFG_TX_SLOT_NUM(v)		((v)<<0)

#define SUNXI_DAUDIOTX0CHSEL					(0x34)
#define SUNXI_DAUDIOTX1CHSEL					(0x38)
#define SUNXI_DAUDIOTX2CHSEL					(0x3C)
#define SUNXI_DAUDIOTX3CHSEL					(0x40)
	#define SUNXI_DAUDIOTXn_OFFSET(v)					((v)<<12)
	#define SUNXI_DAUDIOTXn_CHEN(v)					((v)<<4)
	#define SUNXI_DAUDIOTXn_CHSEL(v)					((v)<<0)
	#define CHEN_MASK								0xff
	#define CHSEL_MASK								0x7
	#define CH_MAX									8

#define SUNXI_DAUDIOTX0CHMAP					(0x44)
#define SUNXI_DAUDIOTX1CHMAP					(0x48)
#define SUNXI_DAUDIOTX2CHMAP					(0x4C)
#define SUNXI_DAUDIOTX3CHMAP					(0x50)
	#define SUNXI_DAUDIOTXn_CH7_MAP				(7<<28)
	#define SUNXI_DAUDIOTXn_CH6_MAP				(7<<24)
	#define SUNXI_DAUDIOTXn_CH5_MAP				(7<<20)
	#define SUNXI_DAUDIOTXn_CH4_MAP				(7<<16)
	#define SUNXI_DAUDIOTXn_CH3_MAP				(7<<12)
	#define SUNXI_DAUDIOTXn_CH2_MAP				(7<<8)
	#define SUNXI_DAUDIOTXn_CH1_MAP				(7<<4)
	#define SUNXI_DAUDIOTXn_CH0_MAP				(7<<0)
	#define SUNXI_TXCHANMAP_DEFAULT							(0x76543210)
#define SUNXI_DAUDIORXCHSEL						(0x54)
	#define SUNXI_DAUDIORXCHSEL_RXOFFSET(v)				((v)<<12)
	#define SUNXI_DAUDIORXCHSEL_RXCHSET(v)				((v)<<0)

#define SUNXI_DAUDIORXCHMAP						(0x58)
	#define SUNXI_DAUDIORXCHMAP_CH7				(7<<28)
	#define SUNXI_DAUDIORXCHMAP_CH6				(7<<24)
	#define SUNXI_DAUDIORXCHMAP_CH5				(7<<20)
	#define SUNXI_DAUDIORXCHMAP_CH4				(7<<16)
	#define SUNXI_DAUDIORXCHMAP_CH3				(7<<12)
	#define SUNXI_DAUDIORXCHMAP_CH2				(7<<8)
	#define SUNXI_DAUDIORXCHMAP_CH1				(7<<4)
	#define SUNXI_DAUDIORXCHMAP_CH0				(7<<0)
#define SUNXI_RXCHANMAP_DEFAULT							(0x76543210)
#define SUNXI_DAUDIODBG							(0x5C)

/*
* fs = samplerate;
* bclk_freq = pll_audio_freq/bclk_div;
* i2s mode: bclk_freq = 2 * pcm_lrck_period * fs;
* make sure:
	i2s mode:channel * slot_width_select <= 2*pcm_lrck_period;
*/
struct _i2s1_bclk_div_s {
	int i2s_lrck_period;
	int pll_audio_freq;
	int sample_rate;
	int bclk_freq;
	int bclk_div;
	int bclk_div_regval;
};

/*
* fs = samplerate;
* bclk_freq = pll_audio_freq/bclk_div;
* pcm mode: bclk_freq = pcm_lrck_period * fs;
* make sure:
	pcm mode:channel * slot_width_select <= pcm_lrck_period;
*/
struct _pcm1_bclk_div_s {
	int pcm_lrck_period;
	int pll_audio_freq;
	int sample_rate;
	int bclk_freq;
	int bclk_div;
	int bclk_div_regval;
};

struct sunxi_daudio_info1 {
	void __iomem   *regs;

	int frametype;			//0: short frame	1: long frame
	int samp_res;			//sample resolution: from 8-bit to 32-bit
	int slot_width;			//slot width: from 8-bit to 32-bit
	int signext;				//0: zero padding	1: sign extension	3: transfer 0 after sample
	int daudio_select;
	int pcm_lrck_period;
	int pcm_lrckr_period;
	int msb_lsb_first;
	int sign_extend;
	int frame_width;
	int tx_data_mode;
	int rx_data_mode;
	int daudio_used;
};
#endif
