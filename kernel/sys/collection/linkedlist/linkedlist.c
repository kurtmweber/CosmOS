//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>

struct linkedlist* linkedlist_new() {
    struct linkedlist* ret = kmalloc(sizeof(struct linkedlist));
    ret->data = 0;
    ret->next = 0;
}

void linkedlist_delete(struct linkedlist* lst) {
    ASSERT_NOT_NULL(lst);
}

uint32_t linkedlist_count(struct linkedlist* lst) {
    ASSERT_NOT_NULL(lst);
}

void linkedlist_add(struct linkedlist* lst, void* value) {
    ASSERT_NOT_NULL(lst);
}
