//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/tree/tree.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/string/string.h>

char G_A[] = {"A"};
char G_B[] = {"BB"};
char G_C[] = {"CCC"};
char G_D[] = {"DDDD"};
char G_E[] = {"EEEEE"};
char G_F[] = {"FFFFFF"};

void test_tree() {
    kprintf("Testing tree\n");

    struct tree* t = tree_new();

    tree_insert(t, 'A', G_A);
    uint8_t* value = tree_search(t, 'A');
    ASSERT(0 == (strcmp(value, G_A)));

    tree_delete(t);
}