
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NTREE_H
#define _NTREE_H

/*
 * an n-ary tree
 */

#include <types.h>
struct ntree {
    void* data;
    struct ntree** childen;
    uint32_t childcount;
};

struct ntree* ntree_new();
void ntree_delete(struct ntree* nt);
uint32_t ntree_childcount(struct ntree* nt);
struct ntree* ntree_child(struct ntree* nt, uint32_t idx);

#endif
