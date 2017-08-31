/*
 * drivers/target/iscsi/iscsi_target_nego.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ISCSI_TARGET_NEGO_H
#define ISCSI_TARGET_NEGO_H

#define DECIMAL         0
#define HEX             1

extern void convert_null_to_semi(char *, int);
extern int extract_param(const char *, const char *, unsigned int, char *,
		unsigned char *);
extern struct iscsi_login *iscsi_target_init_negotiation(
		struct iscsi_np *, struct iscsi_conn *, char *);
extern int iscsi_target_start_negotiation(
		struct iscsi_login *, struct iscsi_conn *);
extern void iscsi_target_nego_release(
		struct iscsi_login *, struct iscsi_conn *);

#endif /* ISCSI_TARGET_NEGO_H */
