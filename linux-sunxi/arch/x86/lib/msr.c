/*
 * arch/x86/lib/msr.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/preempt.h>
#include <asm/msr.h>

struct msr *msrs_alloc(void)
{
	struct msr *msrs = NULL;

	msrs = alloc_percpu(struct msr);
	if (!msrs) {
		pr_warning("%s: error allocating msrs\n", __func__);
		return NULL;
	}

	return msrs;
}
EXPORT_SYMBOL(msrs_alloc);

void msrs_free(struct msr *msrs)
{
	free_percpu(msrs);
}
EXPORT_SYMBOL(msrs_free);
