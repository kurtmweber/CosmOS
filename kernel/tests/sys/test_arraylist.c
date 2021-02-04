//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_arraylist.h>

char C_A[] = {"A"};
char C_B[] = {"BB"};
char C_C[] = {"CCC"};
char C_D[] = {"DDDD"};
char C_E[] = {"EEEEE"};
char C_F[] = {"FFFFFF"};

void test_arraylist_main() {
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

uint8_t arraylist_string_compare(void* e1, void* e2) {
    ASSERT_NOT_NULL(e1);
    ASSERT_NOT_NULL(e2);
    if (strcmp((uint8_t*)e1, (uint8_t*)e2) == 1) {
        kprintf("compare %s %s 1\n", (uint8_t*)e1, (uint8_t*)e2);
        return 1;
    }
    kprintf("compare %s %s 0\n", (uint8_t*)e1, (uint8_t*)e2);
    return 0;
}

void test_arraylist_sort() {
    struct arraylist* lst = arraylist_new();
    arraylist_add(lst, C_A);
    arraylist_add(lst, C_C);
    arraylist_add(lst, C_B);
    arraylist_add(lst, C_D);
    arraylist_add(lst, C_F);
    arraylist_add(lst, C_E);

    for (uint8_t i = 0; i < arraylist_count(lst); i++) {
        //        kprintf("%llu : %s\n", i, (uint8_t*)arraylist_get(lst, i));
    }

    arraylist_sort(lst, &arraylist_string_compare);

    for (uint8_t i = 0; i < arraylist_count(lst); i++) {
        //      kprintf("%llu : %s\n", i, (uint8_t*)arraylist_get(lst, i));
    }

    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 0), C_A) == 0);
    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 1), C_B) == 0);
    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 2), C_C) == 0);
    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 3), C_D) == 0);
    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 4), C_E) == 0);
    ASSERT(strcmp((uint8_t*)arraylist_get(lst, 5), C_F) == 0);

    arraylist_delete(lst);
}

void test_arraylist() {
    test_arraylist_main();
    test_arraylist_sort();
}
