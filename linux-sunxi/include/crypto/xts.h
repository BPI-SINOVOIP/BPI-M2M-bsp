/*
 * include/crypto/xts.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CRYPTO_XTS_H
#define _CRYPTO_XTS_H

#include <crypto/b128ops.h>

struct scatterlist;
struct blkcipher_desc;

#define XTS_BLOCK_SIZE 16

struct xts_crypt_req {
	be128 *tbuf;
	unsigned int tbuflen;

	void *tweak_ctx;
	void (*tweak_fn)(void *ctx, u8* dst, const u8* src);
	void *crypt_ctx;
	void (*crypt_fn)(void *ctx, u8 *blks, unsigned int nbytes);
};

#define XTS_TWEAK_CAST(x) ((void (*)(void *, u8*, const u8*))(x))

int xts_crypt(struct blkcipher_desc *desc, struct scatterlist *dst,
	      struct scatterlist *src, unsigned int nbytes,
	      struct xts_crypt_req *req);

#endif  /* _CRYPTO_XTS_H */
