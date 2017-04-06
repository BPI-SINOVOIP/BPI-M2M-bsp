/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SUNXI_CLK_PERPARE_H
#define _SUNXI_CLK_PERPARE_H

#define CLK_MAX_MODULE_VALUE  15
#define CLK_MAX_ID_VALUE  15

enum CLK_ID_BIT
{
	CLK_USBPHY0_BIT    = (1<<0),
	CLK_USBPHY1_BIT    = (1<<1),
	CLK_USBPHY2_BIT    = (1<<2),
	CLK_USBPHY3_BIT    = (1<<3),
	CLK_USBOHCI0_BIT   = (1<<4),
	CLK_USBOHCI1_BIT   = (1<<5),
	CLK_USBOHCI2_BIT   = (1<<6),
	CLK_USBOHCI3_BIT   = (1<<7),
	CLK_USBEHCI0_BIT   = (1<<8),
	CLK_USBEHCI1_BIT   = (1<<9),
	CLK_USBEHCI2_BIT   = (1<<10),
	CLK_USBEHCI3_BIT   = (1<<11),
	CLK_USBEOTG_BIT    = (1<<12),
	CLK_HDMI_BIT       = (1<<13),
	CLK_ADDDA_COM_BIT  = (1<<14),
	CLK_MAX_INDEX      = (1<<15),
};

char *id_name[CLK_MAX_ID_VALUE] = {
	"usbphy0",
	"usbphy1",
	"usbphy2",
	"usbphy3",
	"usbohci0",
	"usbohci1",
	"usbohci2",
	"usbohci3",
	"usbehci0",
	"usbehci1",
	"usbehci2",
	"usbehci3",
	"usbotg",
	"hdmi",
	"adda_com",
};

char *module_id[CLK_MAX_ID_VALUE] = {
	"usbc0",
	"usbc1",
	"usbc2",
	"usbc3",
	"usbc0",
	"usbc1",
	"usbc2",
	"usbc3",
	"usbc0",
	"usbc1",
	"usbc2",
	"usbc3",
	"usbc0",
	"hdmi_para",
	"audio0",
};

typedef struct {
	unsigned int mask_bit;
	char id_name[20];
}bitmap_name_mapping_t;

bitmap_name_mapping_t clk_bitmap_name_mapping[CLK_MAX_MODULE_VALUE] = {
	{CLK_USBPHY0_BIT|CLK_USBPHY1_BIT|CLK_USBPHY2_BIT|CLK_USBPHY3_BIT|\
		CLK_USBOHCI0_BIT|CLK_USBOHCI1_BIT|CLK_USBOHCI2_BIT|CLK_USBOHCI3_BIT|\
		CLK_USBEHCI0_BIT|CLK_USBEHCI1_BIT|CLK_USBEHCI2_BIT|CLK_USBEHCI3_BIT|\
		CLK_USBEOTG_BIT|CLK_ADDDA_COM_BIT, "hdmi"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
	{0, "NULL"},
};

enum {
	DEBUG_INIT = 1U << 0,
	DEBUG_DATA = 1U << 1,
	DEBUG_TEST = 1U << 2,
};

#define dprintk(level_mask, fmt, arg...)	if (unlikely(debug_mask & level_mask)) \
	 printk(KERN_DEBUG fmt , ## arg)

#endif
