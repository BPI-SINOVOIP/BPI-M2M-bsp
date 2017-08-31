/*
 * drivers/scsi/scsi_lib_dma.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * SCSI library functions depending on DMA
 */

#include <linux/blkdev.h>
#include <linux/device.h>
#include <linux/export.h>
#include <linux/kernel.h>

#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>

/**
 * scsi_dma_map - perform DMA mapping against command's sg lists
 * @cmd:	scsi command
 *
 * Returns the number of sg lists actually used, zero if the sg lists
 * is NULL, or -ENOMEM if the mapping failed.
 */
int scsi_dma_map(struct scsi_cmnd *cmd)
{
	int nseg = 0;

	if (scsi_sg_count(cmd)) {
		struct device *dev = cmd->device->host->dma_dev;

		nseg = dma_map_sg(dev, scsi_sglist(cmd), scsi_sg_count(cmd),
				  cmd->sc_data_direction);
		if (unlikely(!nseg))
			return -ENOMEM;
	}
	return nseg;
}
EXPORT_SYMBOL(scsi_dma_map);

/**
 * scsi_dma_unmap - unmap command's sg lists mapped by scsi_dma_map
 * @cmd:	scsi command
 */
void scsi_dma_unmap(struct scsi_cmnd *cmd)
{
	if (scsi_sg_count(cmd)) {
		struct device *dev = cmd->device->host->dma_dev;

		dma_unmap_sg(dev, scsi_sglist(cmd), scsi_sg_count(cmd),
			     cmd->sc_data_direction);
	}
}
EXPORT_SYMBOL(scsi_dma_unmap);
