//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_arraylist.h>

char Q_A[] = {"A"};
char Q_B[] = {"BB"};
char Q_C[] = {"CCC"};
char Q_D[] = {"DDDD"};
char Q_E[] = {"EEEEE"};
char Q_F[] = {"FFFFFF"};

void test_linkedlist() {
    kprintf("Testing linkedlist\n");

    struct linkedlist* lst = linkedlist_new();
    ASSERT(1 == linkedlist_count(lst));

    linkedlist_add(lst, Q_A);
    ASSERT(2 == linkedlist_count(lst));

    linkedlist_delete(lst);
}
