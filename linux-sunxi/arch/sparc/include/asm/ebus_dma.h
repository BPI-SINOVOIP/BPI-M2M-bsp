/*
 * arch/sparc/include/asm/ebus_dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SPARC_EBUS_DMA_H
#define __ASM_SPARC_EBUS_DMA_H

struct ebus_dma_info {
	spinlock_t	lock;
	void __iomem	*regs;

	unsigned int	flags;
#define EBUS_DMA_FLAG_USE_EBDMA_HANDLER		0x00000001
#define EBUS_DMA_FLAG_TCI_DISABLE		0x00000002

	/* These are only valid is EBUS_DMA_FLAG_USE_EBDMA_HANDLER is
	 * set.
	 */
	void (*callback)(struct ebus_dma_info *p, int event, void *cookie);
	void *client_cookie;
	unsigned int	irq;
#define EBUS_DMA_EVENT_ERROR	1
#define EBUS_DMA_EVENT_DMA	2
#define EBUS_DMA_EVENT_DEVICE	4

	unsigned char	name[64];
};

extern int ebus_dma_register(struct ebus_dma_info *p);
extern int ebus_dma_irq_enable(struct ebus_dma_info *p, int on);
extern void ebus_dma_unregister(struct ebus_dma_info *p);
extern int ebus_dma_request(struct ebus_dma_info *p, dma_addr_t bus_addr,
			    size_t len);
extern void ebus_dma_prepare(struct ebus_dma_info *p, int write);
extern unsigned int ebus_dma_residue(struct ebus_dma_info *p);
extern unsigned int ebus_dma_addr(struct ebus_dma_info *p);
extern void ebus_dma_enable(struct ebus_dma_info *p, int on);

#endif /* __ASM_SPARC_EBUS_DMA_H */
