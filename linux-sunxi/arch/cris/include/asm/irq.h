/*
 * arch/cris/include/asm/irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <arch/irq.h>

static inline int irq_canonicalize(int irq)
{  
  return irq; 
}

#endif  /* _ASM_IRQ_H */


