/*
 * arch/x86/kvm/kvm_timer.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

struct kvm_timer {
	struct hrtimer timer;
	s64 period; 				/* unit: ns */
	u32 timer_mode_mask;
	u64 tscdeadline;
	atomic_t pending;			/* accumulated triggered timers */
	bool reinject;
	struct kvm_timer_ops *t_ops;
	struct kvm *kvm;
	struct kvm_vcpu *vcpu;
};

struct kvm_timer_ops {
	bool (*is_periodic)(struct kvm_timer *);
};

enum hrtimer_restart kvm_timer_fn(struct hrtimer *data);
