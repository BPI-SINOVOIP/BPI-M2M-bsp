/*
 * arch/powerpc/sysdev/mv64x60.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MV64X60_H__
#define __MV64X60_H__

#include <linux/init.h>

extern void __init mv64x60_init_irq(void);
extern unsigned int mv64x60_get_irq(void);

extern void __init mv64x60_pci_init(void);
extern void __init mv64x60_init_early(void);

#endif /* __MV64X60_H__ */
