/*
 * arch/arm/mm/fault.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ARCH_ARM_FAULT_H
#define __ARCH_ARM_FAULT_H

/*
 * Fault status register encodings.  We steal bit 31 for our own purposes.
 */
#define FSR_LNX_PF		(1 << 31)
#define FSR_WRITE		(1 << 11)
#define FSR_FS4			(1 << 10)
#define FSR_FS3_0		(15)
#define FSR_FS5_0		(0x3f)

#ifdef CONFIG_ARM_LPAE
static inline int fsr_fs(unsigned int fsr)
{
	return fsr & FSR_FS5_0;
}
#else
static inline int fsr_fs(unsigned int fsr)
{
	return (fsr & FSR_FS3_0) | (fsr & FSR_FS4) >> 6;
}
#endif

void do_bad_area(unsigned long addr, unsigned int fsr, struct pt_regs *regs);
unsigned long search_exception_table(unsigned long addr);

#endif	/* __ARCH_ARM_FAULT_H */
