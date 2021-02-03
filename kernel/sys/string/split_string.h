//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SPLIT_STRING_H
#define _SPLIT_STRING_H

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

void split_string(uint8_t* str, uint8_t* delim, struct arraylist* al);
void delete_string_list(struct arraylist* al);
#endif