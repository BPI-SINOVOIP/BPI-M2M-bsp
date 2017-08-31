/*
 * net/compat-wireless/net/wireless/sysfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __WIRELESS_SYSFS_H
#define __WIRELESS_SYSFS_H

extern int wiphy_sysfs_init(void);
extern void wiphy_sysfs_exit(void);

extern struct class ieee80211_class;

#endif /* __WIRELESS_SYSFS_H */
