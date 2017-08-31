/*
 * net/mac80211/debugfs_key.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MAC80211_DEBUGFS_KEY_H
#define __MAC80211_DEBUGFS_KEY_H

#ifdef CONFIG_MAC80211_DEBUGFS
void ieee80211_debugfs_key_add(struct ieee80211_key *key);
void ieee80211_debugfs_key_remove(struct ieee80211_key *key);
void ieee80211_debugfs_key_update_default(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_add_mgmt_default(
	struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_remove_mgmt_default(
	struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_key_sta_del(struct ieee80211_key *key,
				   struct sta_info *sta);
#else
static inline void ieee80211_debugfs_key_add(struct ieee80211_key *key)
{}
static inline void ieee80211_debugfs_key_remove(struct ieee80211_key *key)
{}
static inline void ieee80211_debugfs_key_update_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_add_mgmt_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_remove_mgmt_default(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_key_sta_del(struct ieee80211_key *key,
						 struct sta_info *sta)
{}
#endif

#endif /* __MAC80211_DEBUGFS_KEY_H */
