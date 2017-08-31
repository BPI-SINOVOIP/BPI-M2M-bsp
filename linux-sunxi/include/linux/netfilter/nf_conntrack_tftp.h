/*
 * include/linux/netfilter/nf_conntrack_tftp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_CONNTRACK_TFTP_H
#define _NF_CONNTRACK_TFTP_H

#define TFTP_PORT 69

struct tftphdr {
	__be16 opcode;
};

#define TFTP_OPCODE_READ	1
#define TFTP_OPCODE_WRITE	2
#define TFTP_OPCODE_DATA	3
#define TFTP_OPCODE_ACK		4
#define TFTP_OPCODE_ERROR	5

extern unsigned int (*nf_nat_tftp_hook)(struct sk_buff *skb,
				        enum ip_conntrack_info ctinfo,
				        struct nf_conntrack_expect *exp);

#endif /* _NF_CONNTRACK_TFTP_H */
