/*
 * arch/alpha/include/asm/hardirq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ALPHA_HARDIRQ_H
#define _ALPHA_HARDIRQ_H

void ack_bad_irq(unsigned int irq);
#define ack_bad_irq ack_bad_irq

#include <asm-generic/hardirq.h>

#endif /* _ALPHA_HARDIRQ_H */
