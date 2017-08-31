/*
 * include/linux/netfilter/xt_socket.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_SOCKET_H
#define _XT_SOCKET_H

#include <linux/types.h>

enum {
	XT_SOCKET_TRANSPARENT = 1 << 0,
};

struct xt_socket_mtinfo1 {
	__u8 flags;
};

void xt_socket_put_sk(struct sock *sk);
struct sock *xt_socket_get4_sk(const struct sk_buff *skb,
			       struct xt_action_param *par);
struct sock *xt_socket_get6_sk(const struct sk_buff *skb,
			       struct xt_action_param *par);

#endif /* _XT_SOCKET_H */
