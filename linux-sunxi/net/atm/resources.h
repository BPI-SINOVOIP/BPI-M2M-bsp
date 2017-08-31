/*
 * net/atm/resources.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* net/atm/resources.h - ATM-related resources */

/* Written 1995-1998 by Werner Almesberger, EPFL LRC/ICA */


#ifndef NET_ATM_RESOURCES_H
#define NET_ATM_RESOURCES_H

#include <linux/atmdev.h>
#include <linux/mutex.h>


extern struct list_head atm_devs;
extern struct mutex atm_dev_mutex;

int atm_dev_ioctl(unsigned int cmd, void __user *arg, int compat);


#ifdef CONFIG_PROC_FS

#include <linux/proc_fs.h>

void *atm_dev_seq_start(struct seq_file *seq, loff_t *pos);
void atm_dev_seq_stop(struct seq_file *seq, void *v);
void *atm_dev_seq_next(struct seq_file *seq, void *v, loff_t *pos);


int atm_proc_dev_register(struct atm_dev *dev);
void atm_proc_dev_deregister(struct atm_dev *dev);

#else

static inline int atm_proc_dev_register(struct atm_dev *dev)
{
	return 0;
}

static inline void atm_proc_dev_deregister(struct atm_dev *dev)
{
	/* nothing */
}

#endif /* CONFIG_PROC_FS */

int atm_register_sysfs(struct atm_dev *adev, struct device *parent);
void atm_unregister_sysfs(struct atm_dev *adev);
#endif
