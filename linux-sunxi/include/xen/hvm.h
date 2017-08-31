/*
 * include/xen/hvm.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Simple wrappers around HVM functions */
#ifndef XEN_HVM_H__
#define XEN_HVM_H__

#include <xen/interface/hvm/params.h>
#include <asm/xen/hypercall.h>

static inline int hvm_get_parameter(int idx, uint64_t *value)
{
	struct xen_hvm_param xhv;
	int r;

	xhv.domid = DOMID_SELF;
	xhv.index = idx;
	r = HYPERVISOR_hvm_op(HVMOP_get_param, &xhv);
	if (r < 0) {
		printk(KERN_ERR "Cannot get hvm parameter %d: %d!\n",
			idx, r);
		return r;
	}
	*value = xhv.value;
	return r;
}

#define HVM_CALLBACK_VIA_TYPE_VECTOR 0x2
#define HVM_CALLBACK_VIA_TYPE_SHIFT 56
#define HVM_CALLBACK_VECTOR(x) (((uint64_t)HVM_CALLBACK_VIA_TYPE_VECTOR)<<\
		HVM_CALLBACK_VIA_TYPE_SHIFT | (x))

#endif /* XEN_HVM_H__ */
