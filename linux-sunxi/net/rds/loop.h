/*
 * net/rds/loop.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _RDS_LOOP_H
#define _RDS_LOOP_H

/* loop.c */
extern struct rds_transport rds_loop_transport;

void rds_loop_exit(void);

#endif
