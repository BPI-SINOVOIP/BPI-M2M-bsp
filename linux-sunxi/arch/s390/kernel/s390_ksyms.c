/*
 * arch/s390/kernel/s390_ksyms.c
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
#include <linux/kvm_host.h>
#include <asm/ftrace.h>

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(_mcount);
#endif
#if defined(CONFIG_KVM) || defined(CONFIG_KVM_MODULE)
EXPORT_SYMBOL(sie64a);
#endif
