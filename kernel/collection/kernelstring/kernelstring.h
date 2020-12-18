//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _KERNELSTRING_H
#define _KERNELSTRING_H

#include <types.h>

/**
 *  a simple string. The string is null terminated, but stores a length 
 */
typedef struct kernelstring {
	uint8_t* str;
    // does not include terminating null
    uint32_t length;
} kernelstring_t;

struct kernelstring* stringNew();
struct kernelstring* stringNewSized(uint32_t size);
void stringDelete(struct kernelstring* str);
uint32_t stringLength(const struct kernelstring* str);
struct kernelstring* stringFromCStr(const int8_t* str);
const int8_t* stringGetCStr(const struct kernelstring* str);

struct kernelstring* stringItoa3(uint64_t n, uint8_t base);
struct kernelstring* stringConcat(const struct kernelstring* str1, const struct kernelstring* str2);
struct kernelstring* stringCopy(const struct kernelstring* str);
struct kernelstring* stringTrim(const struct kernelstring* str);

#endif