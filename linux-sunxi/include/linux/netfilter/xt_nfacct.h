/*
 * include/linux/netfilter/xt_nfacct.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_NFACCT_MATCH_H
#define _XT_NFACCT_MATCH_H

#include <linux/netfilter/nfnetlink_acct.h>

struct nf_acct;

struct xt_nfacct_match_info {
	char		name[NFACCT_NAME_MAX];
	struct nf_acct	*nfacct;
};

#endif /* _XT_NFACCT_MATCH_H */
