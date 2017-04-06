/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef  __OSAL_POWER_H__
#define  __OSAL_POWER_H__

#include "OSAL.h"
#include <linux/regulator/consumer.h>

#ifndef CONFIG_AW_AXP
#define  __OSAL_POWER_MASK__
#endif

int OSAL_Power_Enable(char *name);

int OSAL_Power_Disable(char *name);

#endif
