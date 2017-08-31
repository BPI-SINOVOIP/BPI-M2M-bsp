/*
 * drivers/net/wireless/rtl818x/rtl8180/rtl8225.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef RTL8180_RTL8225_H
#define RTL8180_RTL8225_H

#define RTL8225_ANAPARAM_ON	0xa0000b59
#define RTL8225_ANAPARAM2_ON	0x860dec11
#define RTL8225_ANAPARAM_OFF	0xa00beb59
#define RTL8225_ANAPARAM2_OFF	0x840dec11

const struct rtl818x_rf_ops * rtl8180_detect_rf(struct ieee80211_hw *);

static inline void rtl8225_write_phy_ofdm(struct ieee80211_hw *dev,
					  u8 addr, u8 data)
{
	rtl8180_write_phy(dev, addr, data);
}

static inline void rtl8225_write_phy_cck(struct ieee80211_hw *dev,
					 u8 addr, u8 data)
{
	rtl8180_write_phy(dev, addr, data | 0x10000);
}

#endif /* RTL8180_RTL8225_H */
