/*
 * drivers/s390/cio/blacklist.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef S390_BLACKLIST_H
#define S390_BLACKLIST_H

extern int is_blacklisted (int ssid, int devno);

#endif
