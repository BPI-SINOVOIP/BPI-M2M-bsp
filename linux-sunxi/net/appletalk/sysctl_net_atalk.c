/*
 * net/appletalk/sysctl_net_atalk.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * sysctl_net_atalk.c: sysctl interface to net AppleTalk subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/atalk directory entry (empty =) ). [MS]
 * Dynamic registration, added aarp entries. (5/30/97 Chris Horn)
 */

#include <linux/sysctl.h>
#include <net/sock.h>
#include <linux/atalk.h>

static struct ctl_table atalk_table[] = {
	{
		.procname	= "aarp-expiry-time",
		.data		= &sysctl_aarp_expiry_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "aarp-tick-time",
		.data		= &sysctl_aarp_tick_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "aarp-retransmit-limit",
		.data		= &sysctl_aarp_retransmit_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "aarp-resolve-time",
		.data		= &sysctl_aarp_resolve_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ },
};

static struct ctl_path atalk_path[] = {
	{ .procname = "net", },
	{ .procname = "appletalk", },
	{ }
};

static struct ctl_table_header *atalk_table_header;

void atalk_register_sysctl(void)
{
	atalk_table_header = register_sysctl_paths(atalk_path, atalk_table);
}

void atalk_unregister_sysctl(void)
{
	unregister_sysctl_table(atalk_table_header);
}
