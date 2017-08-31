/*
 * samples/tracepoints/tracepoint-probe-sample2.c
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
 * tracepoint-probe-sample2.c
 *
 * 2nd sample tracepoint probes.
 */

#include <linux/module.h>
#include <linux/fs.h>
#include "tp-samples-trace.h"

/*
 * Here the caller only guarantees locking for struct file and struct inode.
 * Locking must therefore be done in the probe to use the dentry.
 */
static void probe_subsys_event(void *ignore,
			       struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Event is encountered with inode number %lu\n",
		inode->i_ino);
}

static int __init tp_sample_trace_init(void)
{
	int ret;

	ret = register_trace_subsys_event(probe_subsys_event, NULL);
	WARN_ON(ret);

	return 0;
}

module_init(tp_sample_trace_init);

static void __exit tp_sample_trace_exit(void)
{
	unregister_trace_subsys_event(probe_subsys_event, NULL);
	tracepoint_synchronize_unregister();
}

module_exit(tp_sample_trace_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("Tracepoint Probes Samples");
