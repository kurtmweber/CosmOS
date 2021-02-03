//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/split_string.h>
#include <sys/string/string.h>

void split_string(uint8_t* str, uint8_t* delim, struct arraylist* al) {
    ASSERT_NOT_NULL(str);
    ASSERT_NOT_NULL(delim);
    ASSERT_NOT_NULL(al);
    uint32_t prev = 0;
    uint32_t idx = strstr(str, prev, delim);
    if (0 == idx) {
        /*
        * if the delim appears at the start, skip it
        */
        prev++;
        if (prev < strlen(str)) {
            idx = strstr(str, prev, delim);
        } else {
            idx = -1;
        }
    }
    while (-1 != idx) {
        /*
        * get the current string
        */
        uint8_t* s = kmalloc((idx - prev) + 1);
        substr(str, prev, idx, s, (idx - prev) + 1);
        arraylist_add(al, s);
        prev = idx + 1;
        if (prev < strlen(str)) {
            idx = strstr(str, prev, delim);
        } else {
            idx = -1;
        }
    }
    /*
    * remaining
    */
    uint32_t remaining = (strlen(str) - prev);
    if (remaining > 0) {
        uint8_t* t = kmalloc(remaining + 1);
        substr(str, prev, prev + remaining, t, remaining + 1);
        arraylist_add(al, t);
    }
}

void delete_string_list(struct arraylist* al) {
    ASSERT_NOT_NULL(al);
    for (uint32_t i = 0; i < arraylist_count(al); i++) {
        kfree(arraylist_get(al, i));
    }
    kfree(al);
}
