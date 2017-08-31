/*
 * arch/cris/include/arch-v10/arch/checksum.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CRIS_ARCH_CHECKSUM_H
#define _CRIS_ARCH_CHECKSUM_H

/* Checksum some values used in TCP/UDP headers.
 *
 * The gain by doing this in asm is that C will not generate carry-additions
 * for the 32-bit components of the checksum, so otherwise we would have had
 * to split all of those into 16-bit components, then add.
 */

static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr, unsigned short len,
		   unsigned short proto, __wsum sum)
{
	__wsum res;
	__asm__ ("add.d %2, %0\n\t"
		 "ax\n\t"
		 "add.d %3, %0\n\t"
		 "ax\n\t"
		 "add.d %4, %0\n\t"
		 "ax\n\t"
		 "addq 0, %0\n"
	: "=r" (res)
	: "0" (sum), "r" (daddr), "r" (saddr), "r" ((len + proto) << 8));

	return res;
}	

#endif
