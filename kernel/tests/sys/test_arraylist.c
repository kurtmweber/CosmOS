//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/string/string.h>
#include <tests/sys/test_arraylist.h>

char C_A[] = {"A"};
char C_B[] = {"BB"};
char C_C[] = {"CCC"};
char C_D[] = {"DDDD"};
char C_E[] = {"EEEEE"};
char C_F[] = {"FFFFFF"};

void test_arraylist() {
    kprintf("Testing arraylist\n");

    struct arraylist* lst = arraylist_new();
    arraylist_add(lst, C_A);
    ASSERT(0 == strcmp(arraylist_get(lst, 0), C_A));
    ASSERT(1 == arraylist_count(lst));
    arraylist_add(lst, C_B);
    arraylist_add(lst, C_C);
    arraylist_add(lst, C_D);
    arraylist_add(lst, C_E);
    arraylist_add(lst, C_F);
    ASSERT(6 == arraylist_count(lst));
    ASSERT(0 == strcmp(arraylist_get(lst, 0), C_A));
    ASSERT(0 == strcmp(arraylist_get(lst, 1), C_B));
    ASSERT(0 == strcmp(arraylist_get(lst, 2), C_C));
    ASSERT(0 == strcmp(arraylist_get(lst, 3), C_D));
    ASSERT(0 == strcmp(arraylist_get(lst, 4), C_E));
    ASSERT(0 == strcmp(arraylist_get(lst, 5), C_F));

    arraylist_remove(lst, 2);  // letter C
    ASSERT(5 == arraylist_count(lst));
    ASSERT(0 == strcmp(arraylist_get(lst, 0), C_A));
    ASSERT(0 == strcmp(arraylist_get(lst, 1), C_B));
    ASSERT(0 == strcmp(arraylist_get(lst, 2), C_D));
    ASSERT(0 == strcmp(arraylist_get(lst, 3), C_E));
    ASSERT(0 == strcmp(arraylist_get(lst, 4), C_F));

    arraylist_remove(lst, 4);  // last element, special case
    ASSERT(4 == arraylist_count(lst));
    ASSERT(0 == strcmp(arraylist_get(lst, 0), C_A));
    ASSERT(0 == strcmp(arraylist_get(lst, 1), C_B));
    ASSERT(0 == strcmp(arraylist_get(lst, 2), C_D));
    ASSERT(0 == strcmp(arraylist_get(lst, 3), C_E));

    arraylist_delete(lst);
}
