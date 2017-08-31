/*
 * arch/powerpc/boot/string.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PPC_BOOT_STRING_H_
#define _PPC_BOOT_STRING_H_
#include <stddef.h>

extern char *strcpy(char *dest, const char *src);
extern char *strncpy(char *dest, const char *src, size_t n);
extern char *strcat(char *dest, const char *src);
extern char *strchr(const char *s, int c);
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern size_t strlen(const char *s);
extern size_t strnlen(const char *s, size_t count);

extern void *memset(void *s, int c, size_t n);
extern void *memmove(void *dest, const void *src, unsigned long n);
extern void *memcpy(void *dest, const void *src, unsigned long n);
extern void *memchr(const void *s, int c, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);

#endif	/* _PPC_BOOT_STRING_H_ */
