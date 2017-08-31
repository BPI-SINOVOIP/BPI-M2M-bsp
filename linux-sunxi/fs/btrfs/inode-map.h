/*
 * fs/btrfs/inode-map.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __BTRFS_INODE_MAP
#define __BTRFS_INODE_MAP

void btrfs_init_free_ino_ctl(struct btrfs_root *root);
void btrfs_unpin_free_ino(struct btrfs_root *root);
void btrfs_return_ino(struct btrfs_root *root, u64 objectid);
int btrfs_find_free_ino(struct btrfs_root *root, u64 *objectid);
int btrfs_save_ino_cache(struct btrfs_root *root,
			 struct btrfs_trans_handle *trans);

int btrfs_find_free_objectid(struct btrfs_root *root, u64 *objectid);

#endif
