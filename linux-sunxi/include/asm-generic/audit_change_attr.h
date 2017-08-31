/*
 * include/asm-generic/audit_change_attr.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef __NR_chmod
__NR_chmod,
#endif
__NR_fchmod,
#ifdef __NR_chown
__NR_chown,
__NR_fchown,
__NR_lchown,
#endif
__NR_setxattr,
__NR_lsetxattr,
__NR_fsetxattr,
__NR_removexattr,
__NR_lremovexattr,
__NR_fremovexattr,
#ifdef __NR_fchownat
__NR_fchownat,
__NR_fchmodat,
#endif
#ifdef __NR_chown32
__NR_chown32,
__NR_fchown32,
__NR_lchown32,
#endif
#ifdef __NR_link
__NR_link,
#endif
#ifdef __NR_linkat
__NR_linkat,
#endif
