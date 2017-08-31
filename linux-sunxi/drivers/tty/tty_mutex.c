/*
 * drivers/tty/tty_mutex.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/semaphore.h>
#include <linux/sched.h>

/*
 * The 'big tty mutex'
 *
 * This mutex is taken and released by tty_lock() and tty_unlock(),
 * replacing the older big kernel lock.
 * It can no longer be taken recursively, and does not get
 * released implicitly while sleeping.
 *
 * Don't use in new code.
 */
static DEFINE_MUTEX(big_tty_mutex);

/*
 * Getting the big tty mutex.
 */
void __lockfunc tty_lock(void)
{
	mutex_lock(&big_tty_mutex);
}
EXPORT_SYMBOL(tty_lock);

void __lockfunc tty_unlock(void)
{
	mutex_unlock(&big_tty_mutex);
}
EXPORT_SYMBOL(tty_unlock);
