//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/kernelstring/kernelstring.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_kernel_string.h>

void test_kernel_string() {
    kprintf("Testing kernel string\n");

    struct kernelstring* string1 = string_from_cstr("hello");
    struct kernelstring* string2 = string_from_cstr(" world\n");

    ASSERT(string1->length == 5);
    ASSERT(string2->length == 7);

    struct kernelstring* string3 = string_concat(string1, string2);
    ASSERT(string3->length == 12);
    ASSERT(strcmp(string_get_cstr(string3), "hello world\n") == 0);

    struct kernelstring* string4 = string_itoa3(1000, 16);
    ASSERT(string4->length == 3);
    ASSERT(strcmp(string_get_cstr(string4), "3E8") == 0);

    struct kernelstring* string5 = string_copy(string3);
    ASSERT(string5->length == 12);
    ASSERT(strcmp(string_get_cstr(string5), "hello world\n") == 0);

    struct kernelstring* string7 = string_from_cstr("beer           ");
    ASSERT(string7->length == 15);
    ASSERT(strcmp(string_get_cstr(string7), "beer           ") == 0);

    struct kernelstring* string8 = string_trim(string7);
    ASSERT(string8->length == 4);
    ASSERT(strcmp(string_get_cstr(string8), "beer") == 0);
    string_delete(string1);
    string_delete(string2);
    string_delete(string3);
    string_delete(string4);
    string_delete(string5);
    string_delete(string7);
    string_delete(string8);
    return;
}