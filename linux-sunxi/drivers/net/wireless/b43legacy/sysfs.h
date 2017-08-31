/*
 * drivers/net/wireless/b43legacy/sysfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef B43legacy_SYSFS_H_
#define B43legacy_SYSFS_H_

struct b43legacy_wldev;

int b43legacy_sysfs_register(struct b43legacy_wldev *dev);
void b43legacy_sysfs_unregister(struct b43legacy_wldev *dev);

#endif /* B43legacy_SYSFS_H_ */
