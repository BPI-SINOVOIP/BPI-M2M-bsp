/*
 * arch/sparc/include/asm/dcr.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SPARC64_DCR_H
#define _SPARC64_DCR_H

/* UltraSparc-III/III+ Dispatch Control Register, ASR 0x12 */
#define DCR_DPE		0x0000000000001000 /* III+: D$ Parity Error Enable	*/
#define DCR_OBS		0x0000000000000fc0 /* Observability Bus Controls	*/
#define DCR_BPE		0x0000000000000020 /* Branch Predict Enable		*/
#define DCR_RPE		0x0000000000000010 /* Return Address Prediction Enable	*/
#define DCR_SI		0x0000000000000008 /* Single Instruction Disable	*/
#define DCR_IPE		0x0000000000000004 /* III+: I$ Parity Error Enable	*/
#define DCR_IFPOE	0x0000000000000002 /* IRQ FP Operation Enable		*/
#define DCR_MS		0x0000000000000001 /* Multi-Scalar dispatch		*/

#endif /* _SPARC64_DCR_H */
