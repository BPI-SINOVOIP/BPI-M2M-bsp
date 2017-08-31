/*
 * include/asm-generic/audit_dir_write.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef __NR_rename
__NR_rename,
#endif
#ifdef __NR_mkdir
__NR_mkdir,
#endif
#ifdef __NR_rmdir
__NR_rmdir,
#endif
#ifdef __NR_creat
__NR_creat,
#endif
#ifdef __NR_link
__NR_link,
#endif
#ifdef __NR_unlink
__NR_unlink,
#endif
#ifdef __NR_symlink
__NR_symlink,
#endif
#ifdef __NR_mknod
__NR_mknod,
#endif
#ifdef __NR_mkdirat
__NR_mkdirat,
__NR_mknodat,
__NR_unlinkat,
__NR_renameat,
__NR_linkat,
__NR_symlinkat,
#endif
