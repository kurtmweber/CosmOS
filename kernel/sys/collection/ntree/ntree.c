
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/ntree/ntree.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/assert.h>

struct ntree* ntree_new() {
    struct ntree* ret = (struct ntree*) kmalloc(sizeof(struct ntree));
    ret->childcount=0;
    ret->childen=0;
    ret->data=0;
    return ret;    
}

void ntree_delete(struct ntree* nt) {
    ASSERT_NOT_NULL(nt);

    if (nt->childcount>0){
        for (uint32_t i=0;i<nt->childcount;i++){
            ntree_delete(nt->childen[i]);
        }
    }
    kfree(nt);
}

uint32_t ntree_childcount(struct ntree* nt){
    ASSERT_NOT_NULL(nt);
    return nt->childcount;
}

struct ntree* ntree_child(struct ntree* nt, uint32_t idx) {
    ASSERT_NOT_NULL(nt);
    ASSERT(idx >0,"idx must be greater than zero");
    ASSERT(idx < nt->childcount,"idx must be less than child count");
    return nt->childen[idx];
}

void ntree_add_child(struct ntree* nt, struct ntree* child) {
    ASSERT_NOT_NULL(nt);
    ASSERT_NOT_NULL(child);
    
}


