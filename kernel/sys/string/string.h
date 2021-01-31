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
void uitoa3(uint64_t n, char* s, uint16_t len, uint8_t base);

// string.c
uint64_t strlen(const uint8_t* s);

// note that this allocates a new string!
uint8_t* strtrim(const uint8_t* s);

// len is the byte size of the dest buffer
uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint64_t len);
// len is the byte size of the dest buffer
uint8_t* strncat(uint8_t* dest, const uint8_t* src, uint64_t len);
uint8_t strcmp(const uint8_t* str1, const uint8_t* str2);
// returns -1 if unable to find
uint32_t strstr(const uint8_t* str1, uint32_t start, const uint8_t* str2);
uint8_t* substr(const uint8_t* str1, uint32_t start, uint32_t end, uint8_t* str2, uint32_t size);
#endif