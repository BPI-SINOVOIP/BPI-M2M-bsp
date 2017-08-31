/*
 * drivers/target/iscsi/iscsi_target_device.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ISCSI_TARGET_DEVICE_H
#define ISCSI_TARGET_DEVICE_H

extern void iscsit_determine_maxcmdsn(struct iscsi_session *);
extern void iscsit_increment_maxcmdsn(struct iscsi_cmd *, struct iscsi_session *);

#endif /* ISCSI_TARGET_DEVICE_H */
