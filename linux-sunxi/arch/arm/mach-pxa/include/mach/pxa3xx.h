/*
 * arch/arm/mach-pxa/include/mach/pxa3xx.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __MACH_PXA3XX_H	
#define __MACH_PXA3XX_H

#include <mach/hardware.h>
#include <mach/pxa3xx-regs.h>
#include <mach/irqs.h>

extern void __init pxa3xx_map_io(void);
extern void __init pxa3xx_init_irq(void);
extern void __init pxa95x_init_irq(void);

#define pxa3xx_handle_irq	ichp_handle_irq

#endif /* __MACH_PXA3XX_H */
