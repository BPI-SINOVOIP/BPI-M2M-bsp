/*
 * include/crypto/blowfish.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Common values for blowfish algorithms
 */

#ifndef _CRYPTO_BLOWFISH_H
#define _CRYPTO_BLOWFISH_H

#include <linux/types.h>
#include <linux/crypto.h>

#define BF_BLOCK_SIZE 8
#define BF_MIN_KEY_SIZE 4
#define BF_MAX_KEY_SIZE 56

struct bf_ctx {
	u32 p[18];
	u32 s[1024];
};

int blowfish_setkey(struct crypto_tfm *tfm, const u8 *key,
		    unsigned int key_len);

#endif
