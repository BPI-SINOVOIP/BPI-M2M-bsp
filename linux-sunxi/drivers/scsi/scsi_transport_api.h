/*
 * drivers/scsi/scsi_transport_api.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SCSI_TRANSPORT_API_H
#define _SCSI_TRANSPORT_API_H

void scsi_schedule_eh(struct Scsi_Host *shost);

#endif /* _SCSI_TRANSPORT_API_H */
