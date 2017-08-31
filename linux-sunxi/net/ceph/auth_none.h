/*
 * net/ceph/auth_none.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _FS_CEPH_AUTH_NONE_H
#define _FS_CEPH_AUTH_NONE_H

#include <linux/slab.h>
#include <linux/ceph/auth.h>

/*
 * null security mode.
 *
 * we use a single static authorizer that simply encodes our entity name
 * and global id.
 */

struct ceph_none_authorizer {
	char buf[128];
	int buf_len;
	char reply_buf[0];
};

struct ceph_auth_none_info {
	bool starting;
	bool built_authorizer;
	struct ceph_none_authorizer au;   /* we only need one; it's static */
};

extern int ceph_auth_none_init(struct ceph_auth_client *ac);

#endif

