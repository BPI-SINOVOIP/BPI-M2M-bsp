/*
 * include/linux/netfilter/xt_comment.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_COMMENT_H
#define _XT_COMMENT_H

#define XT_MAX_COMMENT_LEN 256

struct xt_comment_info {
	char comment[XT_MAX_COMMENT_LEN];
};

#endif /* XT_COMMENT_H */
