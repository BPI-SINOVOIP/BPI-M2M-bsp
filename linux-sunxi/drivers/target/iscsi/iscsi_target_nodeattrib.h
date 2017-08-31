/*
 * drivers/target/iscsi/iscsi_target_nodeattrib.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ISCSI_TARGET_NODEATTRIB_H
#define ISCSI_TARGET_NODEATTRIB_H

extern void iscsit_set_default_node_attribues(struct iscsi_node_acl *);
extern int iscsit_na_dataout_timeout(struct iscsi_node_acl *, u32);
extern int iscsit_na_dataout_timeout_retries(struct iscsi_node_acl *, u32);
extern int iscsit_na_nopin_timeout(struct iscsi_node_acl *, u32);
extern int iscsit_na_nopin_response_timeout(struct iscsi_node_acl *, u32);
extern int iscsit_na_random_datain_pdu_offsets(struct iscsi_node_acl *, u32);
extern int iscsit_na_random_datain_seq_offsets(struct iscsi_node_acl *, u32);
extern int iscsit_na_random_r2t_offsets(struct iscsi_node_acl *, u32);
extern int iscsit_na_default_erl(struct iscsi_node_acl *, u32);

#endif /* ISCSI_TARGET_NODEATTRIB_H */
