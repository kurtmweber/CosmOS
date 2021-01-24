//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_string.h>

void test_strncpy() {
    uint8_t source[] = {"this is a test"};
    //   kprintf("source %llu\n", strlen(source));

    // normal copy of the right size from a larger buffer
    uint8_t dest1[5];

    strncpy(dest1, source, 5);
    ASSERT(4 == strlen(dest1));  // should be "test" plus a null termination
    ASSERT(strcmp("this", dest1) == 0);

    // copy small string to large buffer
    uint8_t dest2[225];
    strncpy(dest2, source, 255);
    //  kprintf("source %llu dest %llu\n", strlen(source), strlen(dest2));
    ASSERT(strlen(source) == strlen(dest2));
    ASSERT(strcmp(source, dest2) == 0);
}

void test_string() {
    kprintf("Testing string\n");
    test_strncpy();
}