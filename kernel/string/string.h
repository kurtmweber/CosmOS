/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _STRING_H
#define _STRING_H

#include <types.h>

#define UINT64_DECIMAL_STRING_LENGTH 21

#ifndef _ITOA_C
void uitoa3(uint64_t n, char *s, uint8_t len, uint8_t base);
#endif

#ifndef _STRING_C
uint64_t strlen(const char *s);
char *strtrim(const char *s);
#endif

#endif