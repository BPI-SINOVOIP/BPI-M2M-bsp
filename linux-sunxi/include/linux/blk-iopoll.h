/*
 * include/linux/blk-iopoll.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef BLK_IOPOLL_H
#define BLK_IOPOLL_H

struct blk_iopoll;
typedef int (blk_iopoll_fn)(struct blk_iopoll *, int);

struct blk_iopoll {
	struct list_head list;
	unsigned long state;
	unsigned long data;
	int weight;
	int max;
	blk_iopoll_fn *poll;
};

enum {
	IOPOLL_F_SCHED		= 0,
	IOPOLL_F_DISABLE	= 1,
};

/*
 * Returns 0 if we successfully set the IOPOLL_F_SCHED bit, indicating
 * that we were the first to acquire this iop for scheduling. If this iop
 * is currently disabled, return "failure".
 */
static inline int blk_iopoll_sched_prep(struct blk_iopoll *iop)
{
	if (!test_bit(IOPOLL_F_DISABLE, &iop->state))
		return test_and_set_bit(IOPOLL_F_SCHED, &iop->state);

	return 1;
}

static inline int blk_iopoll_disable_pending(struct blk_iopoll *iop)
{
	return test_bit(IOPOLL_F_DISABLE, &iop->state);
}

extern void blk_iopoll_sched(struct blk_iopoll *);
extern void blk_iopoll_init(struct blk_iopoll *, int, blk_iopoll_fn *);
extern void blk_iopoll_complete(struct blk_iopoll *);
extern void __blk_iopoll_complete(struct blk_iopoll *);
extern void blk_iopoll_enable(struct blk_iopoll *);
extern void blk_iopoll_disable(struct blk_iopoll *);

extern int blk_iopoll_enabled;

#endif
