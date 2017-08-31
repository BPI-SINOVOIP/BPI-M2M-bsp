/*
 * arch/x86/um/mem_64.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "linux/mm.h"
#include "asm/page.h"
#include "asm/mman.h"

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && vma->vm_start == um_vdso_addr)
		return "[vdso]";

	return NULL;
}

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return NULL;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return 0;
}

int in_gate_area_no_mm(unsigned long addr)
{
	return 0;
}
