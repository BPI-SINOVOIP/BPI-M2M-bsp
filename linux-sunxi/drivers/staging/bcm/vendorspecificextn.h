/*
 * drivers/staging/bcm/vendorspecificextn.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __VENDOR_EXTN_NVM_H__
#define __VENDOR_EXTN_NVM_H__

#define CONTINUE_COMMON_PATH 0xFFFF

INT vendorextnGetSectionInfo(PVOID  pContext,PFLASH2X_VENDORSPECIFIC_INFO pVendorInfo);
INT vendorextnExit(PMINI_ADAPTER Adapter);
INT vendorextnInit(PMINI_ADAPTER Adapter);
INT vendorextnIoctl(PMINI_ADAPTER Adapter, UINT cmd, ULONG arg);
INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes);
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes, BOOLEAN bVerify);
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes);

#endif /*  */
