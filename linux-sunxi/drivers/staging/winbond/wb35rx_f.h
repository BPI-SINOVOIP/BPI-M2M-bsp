/*
 * drivers/staging/winbond/wb35rx_f.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __WINBOND_WB35RX_F_H
#define __WINBOND_WB35RX_F_H

#include <net/mac80211.h>
#include "wbhal.h"

//====================================
// Interface function declare
//====================================
unsigned char		Wb35Rx_initial(  struct hw_data * pHwData );
void		Wb35Rx_destroy(  struct hw_data * pHwData );
void		Wb35Rx_stop(  struct hw_data * pHwData );
void		Wb35Rx_start(struct ieee80211_hw *hw);

#endif
