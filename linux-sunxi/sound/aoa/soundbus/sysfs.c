/*
 * sound/aoa/soundbus/sysfs.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/stat.h>
/* FIX UP */
#include "soundbus.h"

#define soundbus_config_of_attr(field, format_string)			\
static ssize_t								\
field##_show (struct device *dev, struct device_attribute *attr,	\
              char *buf)						\
{									\
	struct soundbus_dev *mdev = to_soundbus_device (dev);		\
	return sprintf (buf, format_string, mdev->ofdev.dev.of_node->field); \
}

static ssize_t modalias_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct soundbus_dev *sdev = to_soundbus_device(dev);
	struct platform_device *of = &sdev->ofdev;
	int length;

	if (*sdev->modalias) {
		strlcpy(buf, sdev->modalias, sizeof(sdev->modalias) + 1);
		strcat(buf, "\n");
		length = strlen(buf);
	} else {
		length = sprintf(buf, "of:N%sT%s\n",
				 of->dev.of_node->name, of->dev.of_node->type);
	}

	return length;
}

soundbus_config_of_attr (name, "%s\n");
soundbus_config_of_attr (type, "%s\n");

struct device_attribute soundbus_dev_attrs[] = {
	__ATTR_RO(name),
	__ATTR_RO(type),
	__ATTR_RO(modalias),
	__ATTR_NULL
};
