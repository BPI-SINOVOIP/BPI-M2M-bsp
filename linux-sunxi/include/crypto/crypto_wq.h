/*
 * include/crypto/crypto_wq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef CRYPTO_WQ_H
#define CRYPTO_WQ_H

#include <linux/workqueue.h>

extern struct workqueue_struct *kcrypto_wq;
#endif
