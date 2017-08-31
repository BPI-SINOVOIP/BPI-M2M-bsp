/*
 * drivers/input/touchscreen/gslx680/gslX680.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _GSLX680_H_
#define _GSLX680_H_

#define FILTER_POINT //������Ĭ��6����Ҫ��̫��
#ifdef FILTER_POINT
#define FILTER_MAX	12
#endif

/*������Ե���겻׼�����⣬һ������²�Ҫ������꣬
���ǶԱ�ԵҪ��Ƚ�׼�������ļ���Ҫ�ѵ��������0%*/

struct fw_data
{
    u32 offset : 8;
    u32 : 0;
    u32 val;
};

#endif
