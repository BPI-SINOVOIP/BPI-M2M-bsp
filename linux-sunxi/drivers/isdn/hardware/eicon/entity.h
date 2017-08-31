/*
 * drivers/isdn/hardware/eicon/entity.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* $Id: entity.h,v 1.4 2004/03/21 17:26:01 armin Exp $ */

#ifndef __DIVAS_USER_MODE_IDI_ENTITY__
#define __DIVAS_USER_MODE_IDI_ENTITY__

#define DIVA_UM_IDI_RC_PENDING      0x00000001
#define DIVA_UM_IDI_REMOVE_PENDING  0x00000002
#define DIVA_UM_IDI_TX_FLOW_CONTROL 0x00000004
#define DIVA_UM_IDI_REMOVED         0x00000008
#define DIVA_UM_IDI_ASSIGN_PENDING  0x00000010

typedef struct _divas_um_idi_entity {
	struct list_head          link;
	diva_um_idi_adapter_t *adapter; /* Back to adapter */
	ENTITY e;
	void *os_ref;
	dword status;
	void *os_context;
	int rc_count;
	diva_um_idi_data_queue_t  data; /* definad by user 1 ... MAX */
	diva_um_idi_data_queue_t  rc;   /* two entries */
	BUFFERS                   XData;
	BUFFERS                   RData;
	byte                      buffer[2048 + 512];
} divas_um_idi_entity_t;


#endif
