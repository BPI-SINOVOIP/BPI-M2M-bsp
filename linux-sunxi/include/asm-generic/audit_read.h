/*
 * include/asm-generic/audit_read.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef __NR_readlink
__NR_readlink,
#endif
__NR_quotactl,
__NR_listxattr,
__NR_llistxattr,
__NR_flistxattr,
__NR_getxattr,
__NR_lgetxattr,
__NR_fgetxattr,
#ifdef __NR_readlinkat
__NR_readlinkat,
#endif
