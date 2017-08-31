/*
 * net/compat-wireless/net/mac80211/debugfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MAC80211_DEBUGFS_H
#define __MAC80211_DEBUGFS_H

#ifdef CONFIG_MAC80211_DEBUGFS
extern void debugfs_hw_add(struct ieee80211_local *local);
extern int mac80211_open_file_generic(struct inode *inode, struct file *file);
extern int mac80211_format_buffer(char __user *userbuf, size_t count,
				  loff_t *ppos, char *fmt, ...);
#else
static inline void debugfs_hw_add(struct ieee80211_local *local)
{
}
#endif

#endif /* __MAC80211_DEBUGFS_H */
