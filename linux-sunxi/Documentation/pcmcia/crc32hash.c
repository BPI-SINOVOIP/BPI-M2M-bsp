/*
 * Documentation/pcmcia/crc32hash.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* crc32hash.c - derived from linux/lib/crc32.c, GNU GPL v2 */
/* Usage example:
$ ./crc32hash "Dual Speed"
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

static unsigned int crc32(unsigned char const *p, unsigned int len)
{
	int i;
	unsigned int crc = 0;
	while (len--) {
		crc ^= *p++;
		for (i = 0; i < 8; i++)
			crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320 : 0);
	}
	return crc;
}

int main(int argc, char **argv) {
	unsigned int result;
	if (argc != 2) {
		printf("no string passed as argument\n");
		return -1;
	}
	result = crc32((unsigned char const *)argv[1], strlen(argv[1]));
	printf("0x%x\n", result);
	return 0;
}
