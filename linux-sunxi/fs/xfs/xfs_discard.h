/*
 * fs/xfs/xfs_discard.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef XFS_DISCARD_H
#define XFS_DISCARD_H 1

struct fstrim_range;
struct list_head;

extern int	xfs_ioc_trim(struct xfs_mount *, struct fstrim_range __user *);
extern int	xfs_discard_extents(struct xfs_mount *, struct list_head *);

#endif /* XFS_DISCARD_H */
