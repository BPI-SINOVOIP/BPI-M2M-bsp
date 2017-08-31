/*
 * arch/x86/lib/msr-reg-export.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <asm/msr.h>

EXPORT_SYMBOL(native_rdmsr_safe_regs);
EXPORT_SYMBOL(native_wrmsr_safe_regs);
