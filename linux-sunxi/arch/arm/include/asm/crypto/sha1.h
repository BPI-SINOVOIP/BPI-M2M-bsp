/*
 * arch/arm/include/asm/crypto/sha1.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ASM_ARM_CRYPTO_SHA1_H
#define ASM_ARM_CRYPTO_SHA1_H

#include <linux/crypto.h>
#include <crypto/sha.h>

extern int sha1_update_arm(struct shash_desc *desc, const u8 *data,
			   unsigned int len);

#endif
