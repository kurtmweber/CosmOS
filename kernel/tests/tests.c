//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>

void tests_run() {
    // alway run malloc tests
    // TODO, check this, something might be weird in kfree?
    test_malloc();

    // alway run array tests
    test_array();

    // always run the arraylist tests
    test_arraylist();

    // always run the ringbuffer tests
    test_ringbuffer();

    // always test the linkedlist
    test_linkedlist();

    // always run the tree tests
    test_tree();

    // kernel string
    //  test_kernel_string();

    // always run the string tests
    test_string();

    test_bitmap();

    test_iobuffers();

    test_ntree();

    test_vfs();
    test_gpt();
    //  test_rand();
}
