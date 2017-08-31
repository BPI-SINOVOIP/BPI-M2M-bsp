/*
 * include/linux/netfilter/xt_helper.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_HELPER_H
#define _XT_HELPER_H

struct xt_helper_info {
	int invert;
	char name[30];
};
#endif /* _XT_HELPER_H */
