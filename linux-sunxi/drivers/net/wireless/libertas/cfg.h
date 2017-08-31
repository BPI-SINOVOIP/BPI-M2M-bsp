/*
 * drivers/net/wireless/libertas/cfg.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LBS_CFG80211_H__
#define __LBS_CFG80211_H__

struct device;
struct lbs_private;
struct regulatory_request;
struct wiphy;

struct wireless_dev *lbs_cfg_alloc(struct device *dev);
int lbs_cfg_register(struct lbs_private *priv);
void lbs_cfg_free(struct lbs_private *priv);

int lbs_reg_notifier(struct wiphy *wiphy,
		struct regulatory_request *request);

void lbs_send_disconnect_notification(struct lbs_private *priv);
void lbs_send_mic_failureevent(struct lbs_private *priv, u32 event);

void lbs_scan_done(struct lbs_private *priv);
void lbs_scan_deinit(struct lbs_private *priv);
int lbs_disconnect(struct lbs_private *priv, u16 reason);

#endif
