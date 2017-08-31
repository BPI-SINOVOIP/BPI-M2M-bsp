/*
 * include/linux/fs_stack.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_FS_STACK_H
#define _LINUX_FS_STACK_H

/* This file defines generic functions used primarily by stackable
 * filesystems; none of these functions require i_mutex to be held.
 */

#include <linux/fs.h>

/* externs for fs/stack.c */
extern void fsstack_copy_attr_all(struct inode *dest, const struct inode *src);
extern void fsstack_copy_inode_size(struct inode *dst, struct inode *src);

/* inlines */
static inline void fsstack_copy_attr_atime(struct inode *dest,
					   const struct inode *src)
{
	dest->i_atime = src->i_atime;
}

static inline void fsstack_copy_attr_times(struct inode *dest,
					   const struct inode *src)
{
	dest->i_atime = src->i_atime;
	dest->i_mtime = src->i_mtime;
	dest->i_ctime = src->i_ctime;
}

#endif /* _LINUX_FS_STACK_H */
