/*
 * arch/um/include/asm/setup.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

/* POSIX mandated with _POSIX_ARG_MAX that we can rely on 4096 chars in the
 * command line, so this choice is ok.
 */

#define COMMAND_LINE_SIZE 4096

#endif		/* SETUP_H_INCLUDED */
