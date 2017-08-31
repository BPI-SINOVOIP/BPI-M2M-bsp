/*
 * drivers/staging/comedi/comedi_fops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef _COMEDI_FOPS_H
#define _COMEDI_FOPS_H
#include <linux/types.h>

extern struct class *comedi_class;
extern const struct file_operations comedi_fops;
extern bool comedi_autoconfig;
extern struct comedi_driver *comedi_drivers;

#endif /* _COMEDI_FOPS_H */
