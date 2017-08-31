/*
 * arch/powerpc/sysdev/cpm2_pic.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PPC_KERNEL_CPM2_H
#define _PPC_KERNEL_CPM2_H

extern unsigned int cpm2_get_irq(void);

extern void cpm2_pic_init(struct device_node *);

#endif /* _PPC_KERNEL_CPM2_H */
