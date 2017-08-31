/*
 * drivers/isdn/hardware/eicon/dqueue.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* $Id: dqueue.h,v 1.1.2.2 2001/02/08 12:25:43 armin Exp $ */

#ifndef _DIVA_USER_MODE_IDI_DATA_QUEUE_H__
#define _DIVA_USER_MODE_IDI_DATA_QUEUE_H__

#define DIVA_UM_IDI_MAX_MSGS 64

typedef struct _diva_um_idi_data_queue {
	int segments;
	int max_length;
	int read;
	int write;
	int count;
	int segment_pending;
	void *data[DIVA_UM_IDI_MAX_MSGS];
	int length[DIVA_UM_IDI_MAX_MSGS];
} diva_um_idi_data_queue_t;

int diva_data_q_init(diva_um_idi_data_queue_t *q,
		     int max_length, int max_segments);
int diva_data_q_finit(diva_um_idi_data_queue_t *q);
int diva_data_q_get_max_length(const diva_um_idi_data_queue_t *q);
void *diva_data_q_get_segment4write(diva_um_idi_data_queue_t *q);
void diva_data_q_ack_segment4write(diva_um_idi_data_queue_t *q,
				   int length);
const void *diva_data_q_get_segment4read(const diva_um_idi_data_queue_t *
					 q);
int diva_data_q_get_segment_length(const diva_um_idi_data_queue_t *q);
void diva_data_q_ack_segment4read(diva_um_idi_data_queue_t *q);

#endif
