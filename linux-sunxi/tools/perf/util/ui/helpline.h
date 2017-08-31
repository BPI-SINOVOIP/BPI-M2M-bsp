/*
 * tools/perf/util/ui/helpline.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_UI_HELPLINE_H_
#define _PERF_UI_HELPLINE_H_ 1

#include <stdio.h>
#include <stdarg.h>

void ui_helpline__init(void);
void ui_helpline__pop(void);
void ui_helpline__push(const char *msg);
void ui_helpline__vpush(const char *fmt, va_list ap);
void ui_helpline__fpush(const char *fmt, ...);
void ui_helpline__puts(const char *msg);

extern char ui_helpline__current[];

#endif /* _PERF_UI_HELPLINE_H_ */
