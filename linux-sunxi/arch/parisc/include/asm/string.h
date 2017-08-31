/*
 * arch/parisc/include/asm/string.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PA_STRING_H_
#define _PA_STRING_H_

#define __HAVE_ARCH_MEMSET
extern void * memset(void *, int, size_t);

#define __HAVE_ARCH_MEMCPY
void * memcpy(void * dest,const void *src,size_t count);

#endif
