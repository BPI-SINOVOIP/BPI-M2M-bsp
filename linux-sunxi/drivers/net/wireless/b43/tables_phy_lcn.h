/*
 * drivers/net/wireless/b43/tables_phy_lcn.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef B43_TABLES_PHY_LCN_H_
#define B43_TABLES_PHY_LCN_H_

/* The LCN-PHY tables. */
#define B43_LCNTAB_TYPEMASK		0xF0000000
#define B43_LCNTAB_8BIT			0x10000000
#define B43_LCNTAB_16BIT		0x20000000
#define B43_LCNTAB_32BIT		0x30000000
#define B43_LCNTAB8(table, offset)	(((table) << 10) | (offset) | B43_LCNTAB_8BIT)
#define B43_LCNTAB16(table, offset)	(((table) << 10) | (offset) | B43_LCNTAB_16BIT)
#define B43_LCNTAB32(table, offset)	(((table) << 10) | (offset) | B43_LCNTAB_32BIT)

#define B43_LCNTAB_TX_GAIN_SIZE		128

u32 b43_lcntab_read(struct b43_wldev *dev, u32 offset);
void b43_lcntab_read_bulk(struct b43_wldev *dev, u32 offset,
			  unsigned int nr_elements, void *_data);
void b43_lcntab_write(struct b43_wldev *dev, u32 offset, u32 value);
void b43_lcntab_write_bulk(struct b43_wldev *dev, u32 offset,
			   unsigned int nr_elements, const void *_data);

void b43_phy_lcn_tables_init(struct b43_wldev *dev);

#endif /* B43_TABLES_PHY_LCN_H_ */
