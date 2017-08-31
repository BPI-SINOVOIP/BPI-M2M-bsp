/*
 * arch/alpha/include/asm/hw_irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ALPHA_HW_IRQ_H
#define _ALPHA_HW_IRQ_H


extern volatile unsigned long irq_err_count;
DECLARE_PER_CPU(unsigned long, irq_pmi_count);

#ifdef CONFIG_ALPHA_GENERIC
#define ACTUAL_NR_IRQS	alpha_mv.nr_irqs
#else
#define ACTUAL_NR_IRQS	NR_IRQS
#endif

#endif
