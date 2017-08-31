/*
 * drivers/media/dvb/ttpci/av7110_ipack.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _AV7110_IPACK_H_
#define _AV7110_IPACK_H_

extern int av7110_ipack_init(struct ipack *p, int size,
			     void (*func)(u8 *buf,  int size, void *priv));

extern void av7110_ipack_reset(struct ipack *p);
extern int  av7110_ipack_instant_repack(const u8 *buf, int count, struct ipack *p);
extern void av7110_ipack_free(struct ipack * p);
extern void av7110_ipack_flush(struct ipack *p);

#endif
