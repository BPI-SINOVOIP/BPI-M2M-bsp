/*
 * include/linux/netfilter/xt_qtaguid.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_QTAGUID_MATCH_H
#define _XT_QTAGUID_MATCH_H

/* For now we just replace the xt_owner.
 * FIXME: make iptables aware of qtaguid. */
#include <linux/netfilter/xt_owner.h>

#define XT_QTAGUID_UID    XT_OWNER_UID
#define XT_QTAGUID_GID    XT_OWNER_GID
#define XT_QTAGUID_SOCKET XT_OWNER_SOCKET
#define xt_qtaguid_match_info xt_owner_match_info

#endif /* _XT_QTAGUID_MATCH_H */
