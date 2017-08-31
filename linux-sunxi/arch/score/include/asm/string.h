/*
 * arch/score/include/asm/string.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SCORE_STRING_H
#define _ASM_SCORE_STRING_H

extern void *memset(void *__s, int __c, size_t __count);
extern void *memcpy(void *__to, __const__ void *__from, size_t __n);
extern void *memmove(void *__dest, __const__ void *__src, size_t __n);

#endif /* _ASM_SCORE_STRING_H */
