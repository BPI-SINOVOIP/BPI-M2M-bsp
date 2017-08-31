/*
 * drivers/scsi/scsi_transport_fc_internal.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <scsi/scsi_tgt.h>

#ifdef CONFIG_SCSI_FC_TGT_ATTRS
static inline int fc_tgt_it_nexus_create(struct Scsi_Host *shost, u64 itn_id,
					 char *initiator)
{
	return scsi_tgt_it_nexus_create(shost, itn_id, initiator);
}

static inline int fc_tgt_it_nexus_destroy(struct Scsi_Host *shost, u64 itn_id)
{
	return scsi_tgt_it_nexus_destroy(shost, itn_id);
}
#else
static inline int fc_tgt_it_nexus_create(struct Scsi_Host *shost, u64 itn_id,
					 char *initiator)
{
	return 0;
}

static inline int fc_tgt_it_nexus_destroy(struct Scsi_Host *shost, u64 itn_id)
{
	return 0;
}

#endif
