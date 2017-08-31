/*
 * drivers/gpu/drm/gma500/gem_glue.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
extern void drm_gem_object_release_wrap(struct drm_gem_object *obj);
extern int gem_create_mmap_offset(struct drm_gem_object *obj);
