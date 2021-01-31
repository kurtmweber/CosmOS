//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/split_string.h>
#include <sys/string/string.h>

#include <tests/sys/test_string.h>

void test_split_string() {
    //   kprintf("Testing split_string\n");

    // test 1
    struct arraylist* al1 = arraylist_new();
    split_string("/a/bb/ccc/dddd/", "/", al1);
    ASSERT(4 == arraylist_count(al1));
    delete_string_list(al1);

    // test 2
    struct arraylist* al2 = arraylist_new();
    split_string("/a/bb/ccc/dddd", "/", al2);
    ASSERT(4 == arraylist_count(al2));
    delete_string_list(al2);

    // test 3
    struct arraylist* al3 = arraylist_new();
    split_string("a/bb/ccc/dddd", "/", al3);
    ASSERT(4 == arraylist_count(al3));
    delete_string_list(al3);

    // test 4
    struct arraylist* al4 = arraylist_new();
    split_string("/", "/", al4);
    ASSERT(0 == arraylist_count(al4));
    delete_string_list(al4);
}

void test_substr() {
    //  kprintf("Testing substr\n");

    uint8_t source1[] = {"the rain in spain"};

    // test 1
    uint8_t s1[4];
    substr(source1, 0, 3, s1, 4);
    ASSERT(3 == strlen(s1));
    ASSERT(0 == strcmp(s1, "the"));

    // test 2
    uint8_t s2[30];
    substr(source1, 12, 17, s2, 30);
    ASSERT(5 == strlen(s2));
    ASSERT(0 == strcmp(s2, "spain"));

    // test 3
    uint8_t s3[4];
    substr(source1, 0, 1, s3, 4);
    ASSERT(1 == strlen(s3));
    ASSERT(0 == strcmp(s3, "t"));
}

void test_strstr() {
    //   kprintf("Testing strstr\n");

    // test 1
    uint8_t source1[] = {"the rain in spain"};
    ASSERT(-1 == strstr(source1, 0, "the rain in spain falls"));
    ASSERT(4 == strstr(source1, 0, "rain"));
    ASSERT(-1 == strstr(source1, 0, "beer"));
    ASSERT(0 == strstr(source1, 0, "the rain in spain"));
    ASSERT(4 == strstr(source1, 2, "rain"));

    // test2
    uint8_t source2[] = {"abc"};
    ASSERT(2 == strstr(source2, 0, "c"));

    // test3
    uint8_t source3[] = {"/"};
    ASSERT(0 == strstr(source3, 0, "/"));
}

void test_strtrim() {
    //    kprintf("Testing strtrim\n");
    uint8_t source1[] = {"england expects   "};
    uint8_t* trimmed1 = strtrim(source1);
    ASSERT_NOT_NULL(trimmed1);
    ASSERT(strcmp(trimmed1, "england expects") == 0);
}

void test_strncpy() {
    //   kprintf("Testing strncpy\n");
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
    //   kprintf("Testing strncat\n");

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
    test_strtrim();
    test_strstr();
    test_substr();
    test_split_string();
}