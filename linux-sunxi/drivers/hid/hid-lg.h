/*
 * drivers/hid/hid-lg.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __HID_LG_H
#define __HID_LG_H

#ifdef CONFIG_LOGITECH_FF
int lgff_init(struct hid_device *hdev);
#else
static inline int lgff_init(struct hid_device *hdev) { return -1; }
#endif

#ifdef CONFIG_LOGIRUMBLEPAD2_FF
int lg2ff_init(struct hid_device *hdev);
#else
static inline int lg2ff_init(struct hid_device *hdev) { return -1; }
#endif

#ifdef CONFIG_LOGIG940_FF
int lg3ff_init(struct hid_device *hdev);
#else
static inline int lg3ff_init(struct hid_device *hdev) { return -1; }
#endif

#ifdef CONFIG_LOGIWHEELS_FF
int lg4ff_init(struct hid_device *hdev);
int lg4ff_deinit(struct hid_device *hdev);
#else
static inline int lg4ff_init(struct hid_device *hdev) { return -1; }
static inline int lg4ff_deinit(struct hid_device *hdev) { return -1; }
#endif

#endif
