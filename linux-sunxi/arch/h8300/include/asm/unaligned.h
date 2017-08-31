/*
 * arch/h8300/include/asm/unaligned.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_H8300_UNALIGNED_H
#define _ASM_H8300_UNALIGNED_H

#include <linux/unaligned/be_memmove.h>
#include <linux/unaligned/le_byteshift.h>
#include <linux/unaligned/generic.h>

#define get_unaligned	__get_unaligned_be
#define put_unaligned	__put_unaligned_be

#endif /* _ASM_H8300_UNALIGNED_H */
