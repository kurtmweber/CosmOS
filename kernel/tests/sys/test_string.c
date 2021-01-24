//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
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
void test_strncat() {
    uint8_t source1[] = {"england expects"};
    uint8_t source2[] = {" every man"};

    // a normal example.  buffer is big enough
    uint8_t dest1[255];
    strncpy(dest1, source1, 255);
    strncat(dest1, source2, 255);
    //   debug_show_memblock(dest, 64);
    ASSERT(strcmp(dest1, "england expects every man") == 0);
    ASSERT(strlen(dest1) == strlen(source1) + strlen(source2));

    // an example where the buffer is exactly as big as source1
    uint8_t dest2[16];
    strncpy(dest2, source1, 16);
    ASSERT(strcmp(dest2, source1) == 0);
    ASSERT(strlen(dest2) == 15);  // 15 chars, and one for the terminating null
    // copy, but we cant....
    strncat(dest2, source2, 16);
    //  debug_show_memblock(dest2, 32);
    ASSERT(strcmp(dest2, source1) == 0);

    // an example where the buffer allows some more characters (2 in this test)
    uint8_t dest3[18];
    strncpy(dest3, source1, 18);
    ASSERT(strcmp(dest3, source1) == 0);
    ASSERT(strlen(dest3) == 15);  // 15 chars, and one for the terminating null
    // copy, we'll get 2
    strncat(dest3, source2, 18);
    //  debug_show_memblock(dest3, 32);
    ASSERT(strcmp(dest3, "england expects e") == 0);
}

void test_string() {
    kprintf("Testing string\n");
    test_strncpy();
    test_strncat();
}