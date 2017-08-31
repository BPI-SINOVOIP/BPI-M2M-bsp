/*
 * drivers/target/iscsi/iscsi_target_configfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ISCSI_TARGET_CONFIGFS_H
#define ISCSI_TARGET_CONFIGFS_H

extern int iscsi_target_register_configfs(void);
extern void iscsi_target_deregister_configfs(void);

#endif /* ISCSI_TARGET_CONFIGFS_H */
