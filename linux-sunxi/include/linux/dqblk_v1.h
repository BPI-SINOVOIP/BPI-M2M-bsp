/*
 * include/linux/dqblk_v1.h
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
 *	File with in-memory structures of old quota format
 */

#ifndef _LINUX_DQBLK_V1_H
#define _LINUX_DQBLK_V1_H

/* Root squash turned on */
#define V1_DQF_RSQUASH 1

/* Numbers of blocks needed for updates */
#define V1_INIT_ALLOC 1
#define V1_INIT_REWRITE 1
#define V1_DEL_ALLOC 0
#define V1_DEL_REWRITE 2

#endif	/* _LINUX_DQBLK_V1_H */
