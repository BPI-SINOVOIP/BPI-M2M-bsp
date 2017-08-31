/*
 * arch/ia64/include/asm/iommu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_IOMMU_H
#define _ASM_IA64_IOMMU_H 1

#define cpu_has_x2apic 0
/* 10 seconds */
#define DMAR_OPERATION_TIMEOUT (((cycles_t) local_cpu_data->itc_freq)*10)

extern void pci_iommu_shutdown(void);
extern void no_iommu_init(void);
#ifdef	CONFIG_INTEL_IOMMU
extern int force_iommu, no_iommu;
extern int iommu_pass_through;
extern int iommu_detected;
extern int iommu_group_mf;
#else
#define iommu_pass_through	(0)
#define no_iommu		(1)
#define iommu_detected		(0)
#define iommu_group_mf		(0)
#endif
extern void iommu_dma_init(void);
extern void machvec_init(const char *name);

#endif
