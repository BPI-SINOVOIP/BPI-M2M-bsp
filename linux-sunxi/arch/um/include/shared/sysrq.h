/*
 * arch/um/include/shared/sysrq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_SYSRQ_H
#define __UM_SYSRQ_H

struct task_struct;
extern void show_trace(struct task_struct* task, unsigned long *stack);

#endif
