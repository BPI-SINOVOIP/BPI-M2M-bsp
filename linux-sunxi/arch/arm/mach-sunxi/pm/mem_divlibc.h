/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _MEM_DIVLIBC_H
#define _MEM_DIVLIBC_H

void __mem_div0(void);
__u32 raw_lib_udiv(__u32 dividend, __u32 divisior);
extern void __aeabi_idiv(void);
extern void __aeabi_idivmod(void);
extern void __aeabi_uidiv(void);
extern void __aeabi_uidivmod(void);

#endif /*_MEM_DIVLIBC_H*/

