//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>

struct linkedlist* linkedlist_new() {
    struct linkedlist* ret = kmalloc(sizeof(struct linkedlist));
    ret->data = 0;
    ret->next = 0;
}

/*
 * warning: recursive
 */
void linkedlist_delete(struct linkedlist* lst) {
    ASSERT_NOT_NULL(lst);
    if (0 != lst->next) {
        linkedlist_delete(lst->next);
        kfree(lst);
    }
}

uint32_t linkedlist_count(struct linkedlist* lst) {
    ASSERT_NOT_NULL(lst);
    uint32_t count = 1;
    if (0 != lst->next) {
        lst = lst->next;
        count = count + 1;
    }
    return count;
}

void linkedlist_add(struct linkedlist* lst, void* value) {
    ASSERT_NOT_NULL(lst);
    /*
     * find the end
     */
    while (0 != lst->next) {
        lst = lst->next;
    }
    /*
     * allocate a node and set data
     */
    struct linkedlist* newlist = linkedlist_new();
    newlist->data = value;
    /*
     * connect it in
     */
    lst->next = newlist;
}
