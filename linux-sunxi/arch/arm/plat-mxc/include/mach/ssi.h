/*
 * arch/arm/plat-mxc/include/mach/ssi.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MACH_SSI_H
#define __MACH_SSI_H

struct snd_ac97;

extern unsigned char imx_ssi_fiq_start, imx_ssi_fiq_end;
extern unsigned long imx_ssi_fiq_base, imx_ssi_fiq_tx_buffer, imx_ssi_fiq_rx_buffer;

struct imx_ssi_platform_data {
	unsigned int flags;
#define IMX_SSI_DMA            (1 << 0)
#define IMX_SSI_USE_AC97       (1 << 1)
#define IMX_SSI_NET            (1 << 2)
#define IMX_SSI_SYN            (1 << 3)
#define IMX_SSI_USE_I2S_SLAVE  (1 << 4)
	void (*ac97_reset) (struct snd_ac97 *ac97);
	void (*ac97_warm_reset)(struct snd_ac97 *ac97);
};

#endif /* __MACH_SSI_H */

