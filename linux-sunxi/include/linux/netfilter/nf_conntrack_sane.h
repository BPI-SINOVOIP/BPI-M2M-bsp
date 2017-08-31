/*
 * include/linux/netfilter/nf_conntrack_sane.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_CONNTRACK_SANE_H
#define _NF_CONNTRACK_SANE_H
/* SANE tracking. */

#ifdef __KERNEL__

#define SANE_PORT	6566

enum sane_state {
	SANE_STATE_NORMAL,
	SANE_STATE_START_REQUESTED,
};

/* This structure exists only once per master */
struct nf_ct_sane_master {
	enum sane_state state;
};

#endif /* __KERNEL__ */

#endif /* _NF_CONNTRACK_SANE_H */
