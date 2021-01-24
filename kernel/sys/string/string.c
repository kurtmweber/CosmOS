/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>
#include <types.h>

uint64_t strlen(const uint8_t* s) {
    ASSERT_NOT_NULL(s);
    uint64_t i = 0;

    while (s[i]) {
        i++;
    }

    return i;
}

uint8_t* strtrim(const uint8_t* s) {
    ASSERT_NOT_NULL(s);
    uint64_t i = 0;
    uint64_t j;

    uint8_t* tgt;

    i = strlen(s);

    j = i - 1;

    while ((s[j] == ' ') || (s[j] == '\t')) {
        j--;
    }

    tgt = kmalloc((j + 2) * sizeof(uint8_t));  // +1 for the fact that it's a zero-based index, +1 for the terminator

    for (i = 0; i <= j; i++) {
        tgt[i] = s[i];
    }

    tgt[j + 1] = '\0';
    return tgt;
}

uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint64_t len) {
    ASSERT_NOT_NULL(dest);
    ASSERT_NOT_NULL(src);
    ASSERT(len > 0);
    uint64_t i;
    for (i = 0; ((i < len - 1) && (src[i] != 0)); i++) {
        dest[i] = src[i];
    }
    dest[i] = 0;
    return dest;
}

uint8_t* strcat(uint8_t* dest, const uint8_t* src) {
    ASSERT_NOT_NULL(dest);
    ASSERT_NOT_NULL(src);

    uint16_t i = 0;
    while (dest[i] != 0) {
        i++;
    }
    uint16_t j = 0;
    while (src[j] != 0) {
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j + 1] = 0;
    return dest;
}

uint8_t strcmp(const uint8_t* str1, const uint8_t* str2) {
    ASSERT_NOT_NULL(str1);
    ASSERT_NOT_NULL(str2);
    for (int i = 0;; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
}
