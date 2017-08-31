/*
 * fs/befs/datastream.h
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
 * datastream.h
 *
 */

struct buffer_head *befs_read_datastream(struct super_block *sb,
					 befs_data_stream * ds, befs_off_t pos,
					 uint * off);

int befs_fblock2brun(struct super_block *sb, befs_data_stream * data,
		     befs_blocknr_t fblock, befs_block_run * run);

size_t befs_read_lsymlink(struct super_block *sb, befs_data_stream * data,
			  void *buff, befs_off_t len);

befs_blocknr_t befs_count_blocks(struct super_block *sb, befs_data_stream * ds);

extern const befs_inode_addr BAD_IADDR;

