/*
 * drivers/net/wireless/rtl818x/rtl8187/rfkill.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef RTL8187_RFKILL_H
#define RTL8187_RFKILL_H

void rtl8187_rfkill_init(struct ieee80211_hw *hw);
void rtl8187_rfkill_poll(struct ieee80211_hw *hw);
void rtl8187_rfkill_exit(struct ieee80211_hw *hw);

#endif /* RTL8187_RFKILL_H */
