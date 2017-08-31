/*
 * include/net/esp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NET_ESP_H
#define _NET_ESP_H

#include <linux/skbuff.h>

struct crypto_aead;

struct esp_data {
	/* 0..255 */
	int padlen;

	/* Confidentiality & Integrity */
	struct crypto_aead *aead;
};

extern void *pskb_put(struct sk_buff *skb, struct sk_buff *tail, int len);

struct ip_esp_hdr;

static inline struct ip_esp_hdr *ip_esp_hdr(const struct sk_buff *skb)
{
	return (struct ip_esp_hdr *)skb_transport_header(skb);
}

#endif
