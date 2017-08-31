/*
 * drivers/net/wireless/rtl8723bs_vq0/hal/efuse/efuse_mask.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#if DEV_BUS_TYPE == RT_USB_INTERFACE

	#if defined(CONFIG_RTL8188E)
	#include "rtl8188e/HalEfuseMask8188E_USB.h"
	#endif

	#if defined(CONFIG_RTL8812A)
	#include "rtl8812a/HalEfuseMask8812A_USB.h"
	#endif

	#if defined(CONFIG_RTL8821A)
	#include "rtl8812a/HalEfuseMask8821A_USB.h"
	#endif

	#if defined(CONFIG_RTL8192E)
	#include "rtl8192e/HalEfuseMask8192E_USB.h"
	#endif

	#if defined(CONFIG_RTL8723B)
	#include "rtl8723b/HalEfuseMask8723B_USB.h"
	#endif

	#if defined(CONFIG_RTL8814A)
	#include "rtl8814a/HalEfuseMask8814A_USB.h"
	#endif

#elif DEV_BUS_TYPE == RT_PCI_INTERFACE

	#if defined(CONFIG_RTL8188E)
	#include "rtl8188e/HalEfuseMask8188E_PCIE.h"
	#endif

	#if defined(CONFIG_RTL8812A)
	#include "rtl8812a/HalEfuseMask8812A_PCIE.h"
	#endif

	#if defined(CONFIG_RTL8821A)
	#include "rtl8812a/HalEfuseMask8821A_PCIE.h"
	#endif

	#if defined(CONFIG_RTL8192E)
	#include "rtl8192e/HalEfuseMask8192E_PCIE.h"
	#endif

	#if defined(CONFIG_RTL8723B)
	#include "rtl8723b/HalEfuseMask8723B_PCIE.h"
	#endif

	#if defined(CONFIG_RTL8814A)
	#include "rtl8814a/HalEfuseMask8814A_PCIE.h"
	#endif

#elif DEV_BUS_TYPE == RT_SDIO_INTERFACE

	#if defined(CONFIG_RTL8188E)
	#include "rtl8188e/HalEfuseMask8188E_SDIO.h"
	#endif

#endif