/*
 * arch/mips/include/asm/sn/sn_private.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SN_SN_PRIVATE_H
#define __ASM_SN_SN_PRIVATE_H

#include <asm/sn/types.h>

extern nasid_t master_nasid;

extern void cpu_node_probe(void);
extern cnodeid_t get_compact_nodeid(void);
extern void hub_rtc_init(cnodeid_t);
extern void cpu_time_init(void);
extern void per_cpu_init(void);
extern void install_cpu_nmi_handler(int slice);
extern void install_ipi(void);
extern void setup_replication_mask(void);
extern void replicate_kernel_text(void);
extern pfn_t node_getfirstfree(cnodeid_t);

#endif /* __ASM_SN_SN_PRIVATE_H */
