/*
 * drivers/target/target_core_iblock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef TARGET_CORE_IBLOCK_H
#define TARGET_CORE_IBLOCK_H

#define IBLOCK_VERSION		"4.0"

#define IBLOCK_MAX_CDBS		16
#define IBLOCK_LBA_SHIFT	9

struct iblock_req {
	struct se_task ib_task;
	atomic_t pending;
	atomic_t ib_bio_err_cnt;
} ____cacheline_aligned;

#define IBDF_HAS_UDEV_PATH		0x01

struct iblock_dev {
	unsigned char ibd_udev_path[SE_UDEV_PATH_LEN];
	u32	ibd_flags;
	struct bio_set	*ibd_bio_set;
	struct block_device *ibd_bd;
} ____cacheline_aligned;

#endif /* TARGET_CORE_IBLOCK_H */
