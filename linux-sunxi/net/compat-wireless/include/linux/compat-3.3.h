/*
 * net/compat-wireless/include/linux/compat-3.3.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_3_3_COMPAT_H
#define LINUX_3_3_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))

/* include to override NL80211_FEATURE_SK_TX_STATUS */
#include <linux/nl80211.h>

static inline void skb_complete_wifi_ack(struct sk_buff *skb, bool acked)
{
	WARN_ON(1);
}
#define NL80211_FEATURE_SK_TX_STATUS 0

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)) */

#endif /* LINUX_3_3_COMPAT_H */
