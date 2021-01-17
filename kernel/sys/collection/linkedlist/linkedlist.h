//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
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

/*
 * new list
 */
struct linkedlist* linkedarraylist_new();
/*
 * delete list
 */
void linkedarraylist_delete(struct linkedlist* lst);
/*
 * number items in the list
 */
uint32_t linkedarraylist_count(struct linkedlist* lst);
/*
 * returns zero-based index of added item
 */
uint32_t linkedarraylist_add(struct linkedlist* lst, void* value);

#endif