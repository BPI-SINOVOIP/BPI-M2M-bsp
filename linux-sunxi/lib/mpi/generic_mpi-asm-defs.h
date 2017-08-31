/*
 * lib/mpi/generic_mpi-asm-defs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* This file defines some basic constants for the MPI machinery.  We
 * need to define the types on a per-CPU basis, so it is done with
 * this file here.  */
#define BYTES_PER_MPI_LIMB  (SIZEOF_UNSIGNED_LONG)
