/*
 * include/linux/sirfsoc_dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SIRFSOC_DMA_H_
#define _SIRFSOC_DMA_H_

bool sirfsoc_dma_filter_id(struct dma_chan *chan, void *chan_id);

#endif
