/*
 * drivers/ide/ide-disk.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __IDE_DISK_H
#define __IDE_DISK_H

#include "ide-gd.h"

#ifdef CONFIG_IDE_GD_ATA
/* ide-disk.c */
extern const struct ide_disk_ops ide_ata_disk_ops;
ide_decl_devset(address);
ide_decl_devset(multcount);
ide_decl_devset(nowerr);
ide_decl_devset(wcache);
ide_decl_devset(acoustic);

/* ide-disk_ioctl.c */
int ide_disk_ioctl(ide_drive_t *, struct block_device *, fmode_t, unsigned int,
		   unsigned long);

#ifdef CONFIG_IDE_PROC_FS
/* ide-disk_proc.c */
extern ide_proc_entry_t ide_disk_proc[];
extern const struct ide_proc_devset ide_disk_settings[];
#endif
#else
#define ide_disk_proc		NULL
#define ide_disk_settings	NULL
#endif

#endif /* __IDE_DISK_H */
