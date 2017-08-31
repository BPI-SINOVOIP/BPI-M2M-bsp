/*
 * drivers/media/video/tea6420.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __INCLUDED_TEA6420__
#define __INCLUDED_TEA6420__

/* input pins */
#define TEA6420_OUTPUT1 1
#define TEA6420_OUTPUT2 2
#define TEA6420_OUTPUT3 3
#define TEA6420_OUTPUT4 4

/* output pins */
#define TEA6420_INPUT1 1
#define TEA6420_INPUT2 2
#define TEA6420_INPUT3 3
#define TEA6420_INPUT4 4
#define TEA6420_INPUT5 5
#define TEA6420_INPUT6 6

/* gain on the output pins, ORed with the output pin */
#define TEA6420_GAIN0 0x00
#define TEA6420_GAIN2 0x20
#define TEA6420_GAIN4 0x40
#define TEA6420_GAIN6 0x60

#endif
