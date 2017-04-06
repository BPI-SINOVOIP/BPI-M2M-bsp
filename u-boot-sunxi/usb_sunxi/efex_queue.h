/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef _EFEX_QUEUE_H_
#define _EFEX_QUEUE_H_

#include <common.h>

int efex_queue_init(void);
int efex_queue_exit(void);
int efex_queue_write_one_page( void );
int efex_queue_write_all_page( void );
int efex_save_buff_to_queue(uint flash_start, uint flash_sectors,void* buff);

#endif
