
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/ntree/ntree.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>

struct ntree* ntree_new() {
    struct ntree* ret = (struct ntree*)kmalloc(sizeof(struct ntree));
    ret->children = 0;
    ret->data = 0;
    return ret;
}

void ntree_delete(struct ntree* nt) {
    ASSERT_NOT_NULL(nt);

    if (0 != nt->children) {
        for (uint32_t i = 0; i < arraylist_count(nt->children); i++) {
            ntree_delete(arraylist_get(nt->children, i));
        }
    }
    kfree(nt);
}

uint32_t ntree_childcount(struct ntree* nt) {
    ASSERT_NOT_NULL(nt);
    if (0 != nt->children) {
        return arraylist_count(nt->children);
    } else {
        return 0;
    }
}

struct ntree* ntree_child(struct ntree* nt, uint32_t idx) {
    ASSERT_NOT_NULL(nt);
    ASSERT(idx >= 0);
    ASSERT(idx < ntree_childcount(nt));
    return arraylist_get(nt->children, idx);
}

void ntree_add_child(struct ntree* nt, struct ntree* child) {
    ASSERT_NOT_NULL(nt);
    ASSERT_NOT_NULL(child);
    if (0 == nt->children) {
        nt->children = arraylist_new();
    }
    arraylist_add(nt->children, child);
}
