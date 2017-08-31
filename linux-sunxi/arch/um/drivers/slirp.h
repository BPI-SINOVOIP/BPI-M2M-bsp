/*
 * arch/um/drivers/slirp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_SLIRP_H
#define __UM_SLIRP_H

#include "slip_common.h"

#define SLIRP_MAX_ARGS 100
/*
 * XXX this next definition is here because I don't understand why this
 * initializer doesn't work in slirp_kern.c:
 *
 *   argv :  { init->argv[ 0 ... SLIRP_MAX_ARGS-1 ] },
 *
 * or why I can't typecast like this:
 *
 *   argv :  (char* [SLIRP_MAX_ARGS])(init->argv), 
 */
struct arg_list_dummy_wrapper { char *argv[SLIRP_MAX_ARGS]; };

struct slirp_data {
	void *dev;
	struct arg_list_dummy_wrapper argw;
	int pid;
	int slave;
	struct slip_proto slip;
};

extern const struct net_user_info slirp_user_info;

extern int slirp_user_read(int fd, void *buf, int len, struct slirp_data *pri);
extern int slirp_user_write(int fd, void *buf, int len,
			    struct slirp_data *pri);

#endif
