
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NTREE_H
#define _NTREE_H

/*
 * an n-ary tree
 */

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

struct ntree {
    void* data;
    struct arraylist* children;
};

struct ntree* ntree_new();
void ntree_delete(struct ntree* nt);
uint32_t ntree_childcount(struct ntree* nt);
struct ntree* ntree_child(struct ntree* nt, uint32_t idx);

#endif
