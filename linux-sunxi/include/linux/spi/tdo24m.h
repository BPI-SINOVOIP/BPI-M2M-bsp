/*
 * include/linux/spi/tdo24m.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __TDO24M_H__
#define __TDO24M_H__

enum tdo24m_model {
	TDO24M,
	TDO35S,
};

struct tdo24m_platform_data {
	enum tdo24m_model model;
};

#endif /* __TDO24M_H__ */
