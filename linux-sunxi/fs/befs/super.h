/*
 * fs/befs/super.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * super.h
 */

int befs_load_sb(struct super_block *sb, befs_super_block * disk_sb);

int befs_check_sb(struct super_block *sb);

