/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef  __OSAL_PWM_H__
#define  __OSAL_PWM_H__

#include "OSAL.h"
#include <pwm.h>

//for_test
//#define CONFIG_PWM_SUNXI
#ifndef CONFIG_PWM_SUNXI
#define  __OSAL_PWM_MASK__
#endif

__hdle OSAL_Pwm_request(u32 pwm_id);

int OSAL_Pwm_free(__hdle p_handler);

int OSAL_Pwm_Enable(__hdle p_handler);

int OSAL_Pwm_Disable(__hdle p_handler);

int OSAL_Pwm_Config(__hdle p_handler, int duty_ns, int period_ns);

int OSAL_Pwm_Set_Polarity(__hdle p_handler, int polarity);

#endif
