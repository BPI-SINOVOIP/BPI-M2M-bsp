/*
 * drivers/staging/winbond/wb35tx_f.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __WINBOND_WB35TX_F_H
#define __WINBOND_WB35TX_F_H

#include "core.h"

/*
 * ====================================
 * Interface function declare
 * ====================================
 */
unsigned char Wb35Tx_initial(struct hw_data *hw_data);
void Wb35Tx_destroy(struct hw_data *hw_data);
unsigned char Wb35Tx_get_tx_buffer(struct hw_data *hw_data, u8 **buffer);

void Wb35Tx_EP2VM_start(struct wbsoft_priv *adapter);

void Wb35Tx_start(struct wbsoft_priv *adapter);
void Wb35Tx_stop(struct hw_data *hw_data);

void Wb35Tx_CurrentTime(struct wbsoft_priv *adapter, u32 time_count);

#endif
