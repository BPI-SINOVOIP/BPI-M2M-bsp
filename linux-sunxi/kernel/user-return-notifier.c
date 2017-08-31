/*
 * kernel/user-return-notifier.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/user-return-notifier.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/export.h>

static DEFINE_PER_CPU(struct hlist_head, return_notifier_list);

/*
 * Request a notification when the current cpu returns to userspace.  Must be
 * called in atomic context.  The notifier will also be called in atomic
 * context.
 */
void user_return_notifier_register(struct user_return_notifier *urn)
{
	set_tsk_thread_flag(current, TIF_USER_RETURN_NOTIFY);
	hlist_add_head(&urn->link, &__get_cpu_var(return_notifier_list));
}
EXPORT_SYMBOL_GPL(user_return_notifier_register);

/*
 * Removes a registered user return notifier.  Must be called from atomic
 * context, and from the same cpu registration occurred in.
 */
void user_return_notifier_unregister(struct user_return_notifier *urn)
{
	hlist_del(&urn->link);
	if (hlist_empty(&__get_cpu_var(return_notifier_list)))
		clear_tsk_thread_flag(current, TIF_USER_RETURN_NOTIFY);
}
EXPORT_SYMBOL_GPL(user_return_notifier_unregister);

/* Calls registered user return notifiers */
void fire_user_return_notifiers(void)
{
	struct user_return_notifier *urn;
	struct hlist_node *tmp1, *tmp2;
	struct hlist_head *head;

	head = &get_cpu_var(return_notifier_list);
	hlist_for_each_entry_safe(urn, tmp1, tmp2, head, link)
		urn->on_user_return(urn);
	put_cpu_var(return_notifier_list);
}
