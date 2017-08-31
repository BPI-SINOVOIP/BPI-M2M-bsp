/*
 * kernel/trace/trace_selftest_dynamic.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "trace.h"

int DYN_FTRACE_TEST_NAME(void)
{
	/* used to call mcount */
	return 0;
}

int DYN_FTRACE_TEST_NAME2(void)
{
	/* used to call mcount */
	return 0;
}
