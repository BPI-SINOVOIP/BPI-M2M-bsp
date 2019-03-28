/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef  __OSAL_POWER_H__
#define  __OSAL_POWER_H__

#include "OSAL.h"
//#include <linux/regulator/consumer.h>

#ifndef CONFIG_AW_AXP22
//#define  __OSAL_POWER_MASK__
#endif

int OSAL_Power_Enable(char *name);

int OSAL_Power_Disable(char *name);

#endif
