/*
 * kernel/elfcore.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/elf.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include <asm/elf.h>


Elf_Half __weak elf_core_extra_phdrs(void)
{
	return 0;
}

int __weak elf_core_write_extra_phdrs(struct file *file, loff_t offset, size_t *size,
				      unsigned long limit)
{
	return 1;
}

int __weak elf_core_write_extra_data(struct file *file, size_t *size,
				     unsigned long limit)
{
	return 1;
}

size_t __weak elf_core_extra_data_size(void)
{
	return 0;
}
