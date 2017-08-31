/*
 * include/linux/netfilter/xt_TEE.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_TEE_TARGET_H
#define _XT_TEE_TARGET_H

struct xt_tee_tginfo {
	union nf_inet_addr gw;
	char oif[16];

	/* used internally by the kernel */
	struct xt_tee_priv *priv __attribute__((aligned(8)));
};

#endif /* _XT_TEE_TARGET_H */
