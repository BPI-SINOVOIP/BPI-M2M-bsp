/*
 * include/linux/kobj_map.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * kobj_map.h
 */

#ifndef _KOBJ_MAP_H_
#define _KOBJ_MAP_H_

#include <linux/mutex.h>

typedef struct kobject *kobj_probe_t(dev_t, int *, void *);
struct kobj_map;

int kobj_map(struct kobj_map *, dev_t, unsigned long, struct module *,
	     kobj_probe_t *, int (*)(dev_t, void *), void *);
void kobj_unmap(struct kobj_map *, dev_t, unsigned long);
struct kobject *kobj_lookup(struct kobj_map *, dev_t, int *);
struct kobj_map *kobj_map_init(kobj_probe_t *, struct mutex *);

#endif /* _KOBJ_MAP_H_ */
