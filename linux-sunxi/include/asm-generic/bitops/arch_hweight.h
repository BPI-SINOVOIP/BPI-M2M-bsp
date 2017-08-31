/*
 * include/asm-generic/bitops/arch_hweight.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_GENERIC_BITOPS_ARCH_HWEIGHT_H_
#define _ASM_GENERIC_BITOPS_ARCH_HWEIGHT_H_

#include <asm/types.h>

static inline unsigned int __arch_hweight32(unsigned int w)
{
	return __sw_hweight32(w);
}

static inline unsigned int __arch_hweight16(unsigned int w)
{
	return __sw_hweight16(w);
}

static inline unsigned int __arch_hweight8(unsigned int w)
{
	return __sw_hweight8(w);
}

static inline unsigned long __arch_hweight64(__u64 w)
{
	return __sw_hweight64(w);
}
#endif /* _ASM_GENERIC_BITOPS_HWEIGHT_H_ */
