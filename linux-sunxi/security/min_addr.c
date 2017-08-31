/*
 * security/min_addr.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/security.h>
#include <linux/sysctl.h>

/* amount of vm to protect from userspace access by both DAC and the LSM*/
unsigned long mmap_min_addr;
/* amount of vm to protect from userspace using CAP_SYS_RAWIO (DAC) */
unsigned long dac_mmap_min_addr = CONFIG_DEFAULT_MMAP_MIN_ADDR;
/* amount of vm to protect from userspace using the LSM = CONFIG_LSM_MMAP_MIN_ADDR */

/*
 * Update mmap_min_addr = max(dac_mmap_min_addr, CONFIG_LSM_MMAP_MIN_ADDR)
 */
static void update_mmap_min_addr(void)
{
#ifdef CONFIG_LSM_MMAP_MIN_ADDR
	if (dac_mmap_min_addr > CONFIG_LSM_MMAP_MIN_ADDR)
		mmap_min_addr = dac_mmap_min_addr;
	else
		mmap_min_addr = CONFIG_LSM_MMAP_MIN_ADDR;
#else
	mmap_min_addr = dac_mmap_min_addr;
#endif
}

/*
 * sysctl handler which just sets dac_mmap_min_addr = the new value and then
 * calls update_mmap_min_addr() so non MAP_FIXED hints get rounded properly
 */
int mmap_min_addr_handler(struct ctl_table *table, int write,
			  void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;

	if (write && !capable(CAP_SYS_RAWIO))
		return -EPERM;

	ret = proc_doulongvec_minmax(table, write, buffer, lenp, ppos);

	update_mmap_min_addr();

	return ret;
}

static int __init init_mmap_min_addr(void)
{
	update_mmap_min_addr();

	return 0;
}
pure_initcall(init_mmap_min_addr);
