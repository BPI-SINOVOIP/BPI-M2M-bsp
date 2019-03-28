/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef  __OSAL_PARSER_H__
#define  __OSAL_PARSER_H__

#include "OSAL.h"

int OSAL_Script_FetchParser_Data(char *main_name, char *sub_name, int value[], int count);
int OSAL_Script_FetchParser_Data_Ex(char *main_name, char *sub_name, int value[], script_parser_value_type_t *type, int count);
int OSAL_sw_get_ic_ver(void);

#endif
