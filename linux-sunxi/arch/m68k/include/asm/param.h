/*
 * arch/m68k/include/asm/param.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _M68K_PARAM_H
#define _M68K_PARAM_H

#ifdef __uClinux__
#define EXEC_PAGESIZE	4096
#else
#define EXEC_PAGESIZE	8192
#endif

#include <asm-generic/param.h>

#endif /* _M68K_PARAM_H */
