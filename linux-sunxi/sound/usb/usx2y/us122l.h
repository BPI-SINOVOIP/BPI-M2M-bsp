/*
 * sound/usb/usx2y/us122l.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef US122L_H
#define US122L_H


struct us122l {
	struct usb_device	*dev;
	int			card_index;
	int			stride;
	struct usb_stream_kernel sk;

	struct mutex		mutex;
	struct file		*first;
	unsigned		second_periods_polled;
	struct file		*master;
	struct file		*slave;
	struct list_head	midi_list;

	atomic_t		mmap_count;
};


#define US122L(c) ((struct us122l *)(c)->private_data)

#define NAME_ALLCAPS "US-122L"

#define USB_ID_US122L 0x800E
#define USB_ID_US144 0x800F
#define USB_ID_US122MKII 0x8021
#define USB_ID_US144MKII 0x8020

#endif
