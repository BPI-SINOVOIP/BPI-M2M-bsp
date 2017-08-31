/*
 * include/asm-generic/bitops/ext2-atomic.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_GENERIC_BITOPS_EXT2_ATOMIC_H_
#define _ASM_GENERIC_BITOPS_EXT2_ATOMIC_H_

/*
 * Spinlock based version of ext2 atomic bitops
 */

#define ext2_set_bit_atomic(lock, nr, addr)		\
	({						\
		int ret;				\
		spin_lock(lock);			\
		ret = __test_and_set_bit_le(nr, addr);	\
		spin_unlock(lock);			\
		ret;					\
	})

#define ext2_clear_bit_atomic(lock, nr, addr)		\
	({						\
		int ret;				\
		spin_lock(lock);			\
		ret = __test_and_clear_bit_le(nr, addr);	\
		spin_unlock(lock);			\
		ret;					\
	})

#endif /* _ASM_GENERIC_BITOPS_EXT2_ATOMIC_H_ */
