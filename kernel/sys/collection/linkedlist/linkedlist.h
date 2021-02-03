//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _LIST_H
#define _LIST_H

#include <types.h>

struct linkedlist {
    struct linkedlist* next;
    void* data;
};

struct linkedlist* linkedlist_new();
void linkedlist_delete(struct linkedlist* lst);
uint32_t linkedlist_count(struct linkedlist* lst);
void linkedlist_add(struct linkedlist* lst, void* value);

#endif