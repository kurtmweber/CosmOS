//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _STRING_H
#define _STRING_H

#include <collection/array/array.h>

/**
 *  a simple string based on an array. The string is null terminated, but stores a length 
 */
typedef struct string {
	struct array* arr;
    uint16_t length;
} string_t;

struct string* stringNew();
void stringDelete(struct string* str);

#endif