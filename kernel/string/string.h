/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _STRING_H
#define _STRING_H

#include <types.h>

// itoa.c
void uitoa3(uint64_t n, char *s, uint16_t len, uint8_t base);

// string.c
uint64_t strlen(const char *s);
char *strtrim(const char *s);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
char strcmp(const char *str1, const char *str2);

#endif