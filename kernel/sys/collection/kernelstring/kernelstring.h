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
    int8_t* str;
    // does not include terminating null
    uint32_t length;
} kernelstring_t;

struct kernelstring* string_new();
struct kernelstring* string_new_sized(uint32_t size);
void string_delete(struct kernelstring* str);
uint32_t string_length(const struct kernelstring* str);
struct kernelstring* string_from_cstr(const int8_t* str);
const int8_t* string_get_cstr(const struct kernelstring* str);

struct kernelstring* string_itoa3(uint64_t n, uint8_t base);
struct kernelstring* string_concat(const struct kernelstring* str1, const struct kernelstring* str2);
struct kernelstring* string_copy(const struct kernelstring* str);
struct kernelstring* string_trim(const struct kernelstring* str);

#endif