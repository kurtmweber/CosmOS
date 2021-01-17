
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TREE_H
#define _TREE_H

#include <types.h>

struct tree {
    void* value;
    uint32_t key;
    struct tree* left;
    struct tree* right;
};

struct tree* tree_new();
void tree_delete(struct tree* t);
void tree_insert(struct tree* t, uint32_t key, void* value);
void* tree_search(struct tree* t, uint32_t key);

#endif
