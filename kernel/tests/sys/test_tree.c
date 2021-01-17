//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/tree/tree.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>

void test_tree() {
    kprintf("Testing tree\n");

    struct tree* t = tree_new();

    tree_delete(t);
}