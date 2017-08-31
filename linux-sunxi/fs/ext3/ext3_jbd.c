/*
 * fs/ext3/ext3_jbd.c
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
 * Interface between ext3 and JBD
 */

#include "ext3.h"

int __ext3_journal_get_undo_access(const char *where, handle_t *handle,
				struct buffer_head *bh)
{
	int err = journal_get_undo_access(handle, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}

int __ext3_journal_get_write_access(const char *where, handle_t *handle,
				struct buffer_head *bh)
{
	int err = journal_get_write_access(handle, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}

int __ext3_journal_forget(const char *where, handle_t *handle,
				struct buffer_head *bh)
{
	int err = journal_forget(handle, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}

int __ext3_journal_revoke(const char *where, handle_t *handle,
				unsigned long blocknr, struct buffer_head *bh)
{
	int err = journal_revoke(handle, blocknr, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}

int __ext3_journal_get_create_access(const char *where,
				handle_t *handle, struct buffer_head *bh)
{
	int err = journal_get_create_access(handle, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}

int __ext3_journal_dirty_metadata(const char *where,
				handle_t *handle, struct buffer_head *bh)
{
	int err = journal_dirty_metadata(handle, bh);
	if (err)
		ext3_journal_abort_handle(where, __func__, bh, handle,err);
	return err;
}
