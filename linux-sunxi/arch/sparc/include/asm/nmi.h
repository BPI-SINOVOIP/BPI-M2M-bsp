/*
 * arch/sparc/include/asm/nmi.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __NMI_H
#define __NMI_H

extern int __init nmi_init(void);
extern void perfctr_irq(int irq, struct pt_regs *regs);
extern void nmi_adjust_hz(unsigned int new_hz);

extern atomic_t nmi_active;

extern void start_nmi_watchdog(void *unused);
extern void stop_nmi_watchdog(void *unused);

#endif /* __NMI_H */
