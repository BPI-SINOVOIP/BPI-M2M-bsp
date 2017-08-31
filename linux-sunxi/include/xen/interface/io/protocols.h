/*
 * include/xen/interface/io/protocols.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __XEN_PROTOCOLS_H__
#define __XEN_PROTOCOLS_H__

#define XEN_IO_PROTO_ABI_X86_32     "x86_32-abi"
#define XEN_IO_PROTO_ABI_X86_64     "x86_64-abi"
#define XEN_IO_PROTO_ABI_IA64       "ia64-abi"
#define XEN_IO_PROTO_ABI_POWERPC64  "powerpc64-abi"

#if defined(__i386__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_X86_32
#elif defined(__x86_64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_X86_64
#elif defined(__ia64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_IA64
#elif defined(__powerpc64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_POWERPC64
#else
# error arch fixup needed here
#endif

#endif
