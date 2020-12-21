/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _STRING_H
#define _STRING_H

#include <types.h>

void uitoa3(uint64_t n, uint8_t *s, uint16_t len, uint8_t base);
uint64_t strlen(const char *s);
char *strtrim(const char *s);
char *strcpy(char *dest, const char *src);

#endif