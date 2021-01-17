//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/string/string.h>
#include <tests/sys/test_arraylist.h>

void test_linkedlist() {
    kprintf("Testing linkedlist\n");

    struct linkedlist* lst = linkedlist_new();

    linkedlist_delete(lst);
}
