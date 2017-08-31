/*
 * arch/c6x/include/asm/sections.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_C6X_SECTIONS_H
#define _ASM_C6X_SECTIONS_H

#include <asm-generic/sections.h>

extern char _vectors_start[];
extern char _vectors_end[];

extern char _data_lma[];
extern char _fdt_start[], _fdt_end[];

#endif /* _ASM_C6X_SECTIONS_H */
