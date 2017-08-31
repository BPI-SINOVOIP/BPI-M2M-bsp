/*
 * include/linux/cryptohash.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __CRYPTOHASH_H
#define __CRYPTOHASH_H

#define SHA_DIGEST_WORDS 5
#define SHA_MESSAGE_BYTES (512 /*bits*/ / 8)
#define SHA_WORKSPACE_WORDS 16

void sha_init(__u32 *buf);
void sha_transform(__u32 *digest, const char *data, __u32 *W);

#define MD5_DIGEST_WORDS 4
#define MD5_MESSAGE_BYTES 64

void md5_transform(__u32 *hash, __u32 const *in);

__u32 half_md4_transform(__u32 buf[4], __u32 const in[8]);

#endif
