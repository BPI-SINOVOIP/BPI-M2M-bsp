/*
 * drivers/zorro/zorro.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

extern void zorro_name_device(struct zorro_dev *z);
extern int zorro_create_sysfs_dev_files(struct zorro_dev *z);

