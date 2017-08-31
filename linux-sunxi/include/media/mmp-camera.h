/*
 * include/media/mmp-camera.h
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
 * Information for the Marvell Armada MMP camera
 */

struct mmp_camera_platform_data {
	struct platform_device *i2c_device;
	int sensor_power_gpio;
	int sensor_reset_gpio;
};
