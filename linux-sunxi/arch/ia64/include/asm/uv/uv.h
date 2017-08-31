/*
 * arch/ia64/include/asm/uv/uv.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_UV_UV_H
#define _ASM_IA64_UV_UV_H

#include <asm/sn/simulator.h>

static inline int is_uv_system(void)
{
	/* temporary support for running on hardware simulator */
	return IS_MEDUSA() || ia64_platform_is("uv");
}

#endif	/* _ASM_IA64_UV_UV_H */
