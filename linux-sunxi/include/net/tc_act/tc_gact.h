/*
 * include/net/tc_act/tc_gact.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __NET_TC_GACT_H
#define __NET_TC_GACT_H

#include <net/act_api.h>

struct tcf_gact {
	struct tcf_common	common;
#ifdef CONFIG_GACT_PROB
        u16			tcfg_ptype;
        u16			tcfg_pval;
        int			tcfg_paction;
#endif
};
#define to_gact(pc) \
	container_of(pc, struct tcf_gact, common)

#endif /* __NET_TC_GACT_H */
