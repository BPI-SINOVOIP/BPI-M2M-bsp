/*
 * net/compat-wireless/compat/compat_atomic.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/spinlock.h>
#include <linux/module.h>

#if !((LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)) && (defined(CONFIG_UML) || defined(CONFIG_X86))) && !((LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33)) && defined(CONFIG_ARM) && !defined(CONFIG_GENERIC_ATOMIC64))

static DEFINE_SPINLOCK(lock);

long long atomic64_read(const atomic64_t *v)
{
    unsigned long flags;
    long long val;

    spin_lock_irqsave(&lock, flags);
    val = v->counter;
    spin_unlock_irqrestore(&lock, flags);
    return val;
}
EXPORT_SYMBOL(atomic64_read);

long long atomic64_add_return(long long a, atomic64_t *v)
{
    unsigned long flags;
    long long val;

    spin_lock_irqsave(&lock, flags);
    val = v->counter += a;
    spin_unlock_irqrestore(&lock, flags);
    return val;
}
EXPORT_SYMBOL(atomic64_add_return);

#endif

