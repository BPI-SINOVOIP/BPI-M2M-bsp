/*
 * include/net/netfilter/xt_log.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#define S_SIZE (1024 - (sizeof(unsigned int) + 1))

struct sbuff {
	unsigned int	count;
	char		buf[S_SIZE + 1];
};
static struct sbuff emergency, *emergency_ptr = &emergency;

static __printf(2, 3) int sb_add(struct sbuff *m, const char *f, ...)
{
	va_list args;
	int len;

	if (likely(m->count < S_SIZE)) {
		va_start(args, f);
		len = vsnprintf(m->buf + m->count, S_SIZE - m->count, f, args);
		va_end(args);
		if (likely(m->count + len < S_SIZE)) {
			m->count += len;
			return 0;
		}
	}
	m->count = S_SIZE;
	printk_once(KERN_ERR KBUILD_MODNAME " please increase S_SIZE\n");
	return -1;
}

static struct sbuff *sb_open(void)
{
	struct sbuff *m = kmalloc(sizeof(*m), GFP_ATOMIC);

	if (unlikely(!m)) {
		local_bh_disable();
		do {
			m = xchg(&emergency_ptr, NULL);
		} while (!m);
	}
	m->count = 0;
	return m;
}

static void sb_close(struct sbuff *m)
{
	m->buf[m->count] = 0;
	printk("%s\n", m->buf);

	if (likely(m != &emergency))
		kfree(m);
	else {
		emergency_ptr = m;
		local_bh_enable();
	}
}

