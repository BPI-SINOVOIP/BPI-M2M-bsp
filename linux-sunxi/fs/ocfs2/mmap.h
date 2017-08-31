/*
 * fs/ocfs2/mmap.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef OCFS2_MMAP_H
#define OCFS2_MMAP_H

int ocfs2_mmap(struct file *file, struct vm_area_struct *vma);

#endif  /* OCFS2_MMAP_H */
