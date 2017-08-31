/*
 * drivers/vhost/test.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_VHOST_TEST_H
#define LINUX_VHOST_TEST_H

/* Start a given test on the virtio null device. 0 stops all tests. */
#define VHOST_TEST_RUN _IOW(VHOST_VIRTIO, 0x31, int)

#endif
