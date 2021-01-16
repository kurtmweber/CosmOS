//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>

void tests_run() {
    // always run the list tests
    test_list();

    // always run the string tests
    test_string();
}
