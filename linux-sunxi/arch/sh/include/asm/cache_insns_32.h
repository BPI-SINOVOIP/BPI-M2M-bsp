/*
 * arch/sh/include/asm/cache_insns_32.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_CACHE_INSNS_32_H
#define __ASM_SH_CACHE_INSNS_32_H

#include <linux/types.h>

#if defined(CONFIG_CPU_SH4A)
#define __icbi(addr)	__asm__ __volatile__ ( "icbi @%0\n\t" : : "r" (addr))
#else
#define __icbi(addr)	mb()
#endif

#define __ocbp(addr)	__asm__ __volatile__ ( "ocbp @%0\n\t" : : "r" (addr))
#define __ocbi(addr)	__asm__ __volatile__ ( "ocbi @%0\n\t" : : "r" (addr))
#define __ocbwb(addr)	__asm__ __volatile__ ( "ocbwb @%0\n\t" : : "r" (addr))

static inline reg_size_t register_align(void *val)
{
	return (unsigned long)(signed long)val;
}

#endif /* __ASM_SH_CACHE_INSNS_32_H */
