/*
 * include/linux/oxu210hp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* platform data for the OXU210HP HCD */

struct oxu210hp_platform_data {
	unsigned int bus16:1;
	unsigned int use_hcd_otg:1;
	unsigned int use_hcd_sph:1;
};
