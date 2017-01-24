/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __HDMI_HW_INTERFACE_H_
#define __HDMI_HW_INTERFACE_H_


struct sunxi_audio_para
{
    __s32   audio_en;
    __s32   sample_rate;
    __s32   channel_num;

    __s32   CTS;
    __s32   ACR_N;
    __s32   CH_STATUS0;
    __s32   CH_STATUS1;
    __u8    data_raw;		/*0:pcm;1:raw*/
    __u8    sample_bit;/* 8/16/20/24/32 bit */
    __u8    ca; /* channel allocation */
};

#endif /* API_H_ */

