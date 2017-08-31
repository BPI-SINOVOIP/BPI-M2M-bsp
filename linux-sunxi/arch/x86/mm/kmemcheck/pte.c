/*
 * arch/x86/mm/kmemcheck/pte.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/mm.h>

#include <asm/pgtable.h>

#include "pte.h"

pte_t *kmemcheck_pte_lookup(unsigned long address)
{
	pte_t *pte;
	unsigned int level;

	pte = lookup_address(address, &level);
	if (!pte)
		return NULL;
	if (level != PG_LEVEL_4K)
		return NULL;
	if (!pte_hidden(*pte))
		return NULL;

	return pte;
}

