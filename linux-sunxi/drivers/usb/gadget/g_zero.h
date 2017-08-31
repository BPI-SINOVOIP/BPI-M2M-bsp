/*
 * drivers/usb/gadget/g_zero.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * This header declares the utility functions used by "Gadget Zero", plus
 * interfaces to its two single-configuration function drivers.
 */

#ifndef __G_ZERO_H
#define __G_ZERO_H

#include <linux/usb/composite.h>

/* global state */
extern unsigned buflen;
extern const struct usb_descriptor_header *otg_desc[];

/* common utilities */
struct usb_request *alloc_ep_req(struct usb_ep *ep);
void free_ep_req(struct usb_ep *ep, struct usb_request *req);
void disable_endpoints(struct usb_composite_dev *cdev,
		struct usb_ep *in, struct usb_ep *out);

/* configuration-specific linkup */
int sourcesink_add(struct usb_composite_dev *cdev, bool autoresume);
int loopback_add(struct usb_composite_dev *cdev, bool autoresume);

#endif /* __G_ZERO_H */
