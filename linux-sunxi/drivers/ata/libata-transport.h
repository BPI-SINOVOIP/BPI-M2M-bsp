/*
 * drivers/ata/libata-transport.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LIBATA_TRANSPORT_H
#define _LIBATA_TRANSPORT_H


extern struct scsi_transport_template *ata_scsi_transport_template;

int ata_tlink_add(struct ata_link *link);
void ata_tlink_delete(struct ata_link *link);

int ata_tport_add(struct device *parent, struct ata_port *ap);
void ata_tport_delete(struct ata_port *ap);

struct scsi_transport_template *ata_attach_transport(void);
void ata_release_transport(struct scsi_transport_template *t);

__init int libata_transport_init(void);
void __exit libata_transport_exit(void);
#endif
