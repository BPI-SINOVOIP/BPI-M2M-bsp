/*
 * include/scsi/scsi_dbg.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SCSI_SCSI_DBG_H
#define _SCSI_SCSI_DBG_H

struct scsi_cmnd;
struct scsi_sense_hdr;

extern void scsi_print_command(struct scsi_cmnd *);
extern void __scsi_print_command(unsigned char *);
extern void scsi_show_extd_sense(unsigned char, unsigned char);
extern void scsi_show_sense_hdr(struct scsi_sense_hdr *);
extern void scsi_print_sense_hdr(const char *, struct scsi_sense_hdr *);
extern void scsi_cmd_print_sense_hdr(struct scsi_cmnd *, const char *,
				     struct scsi_sense_hdr *);
extern void scsi_print_sense(char *, struct scsi_cmnd *);
extern void __scsi_print_sense(const char *name,
			       const unsigned char *sense_buffer,
			       int sense_len);
extern void scsi_show_result(int);
extern void scsi_print_result(struct scsi_cmnd *);
extern void scsi_print_status(unsigned char);
extern const char *scsi_sense_key_string(unsigned char);
extern const char *scsi_extd_sense_format(unsigned char, unsigned char);

#endif /* _SCSI_SCSI_DBG_H */
