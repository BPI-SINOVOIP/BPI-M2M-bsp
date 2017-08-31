/*
 * arch/x86/include/asm/delay.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_DELAY_H
#define _ASM_X86_DELAY_H

#include <asm-generic/delay.h>

void use_tsc_delay(void);

#endif /* _ASM_X86_DELAY_H */
