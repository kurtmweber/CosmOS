//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/ntree/ntree.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_ntree.h>

void test_ntree() {
    kprintf("Testing ntree\n");

    struct ntree* nt = ntree_new();

    ntree_delete(nt);
}