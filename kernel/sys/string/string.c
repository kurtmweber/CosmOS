/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

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

    tgt[j + 1] = 0;
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

uint8_t* strncat(uint8_t* dest, const uint8_t* src, uint64_t len) {
    ASSERT_NOT_NULL(dest);
    ASSERT_NOT_NULL(src);
    ASSERT(len > 0);

    // walk to end of dest
    uint64_t i = 0;
    while (dest[i] != 0) {
        i++;
    }

    uint64_t j = 0;
    while (((i + j) < (len - 1)) && (src[j] != 0)) {
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j] = 0;
    return dest;
}

uint8_t strcmp(const uint8_t* str1, const uint8_t* str2) {
    ASSERT_NOT_NULL(str1);
    ASSERT_NOT_NULL(str2);
    for (uint64_t i = 0;; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (str1[i] == 0) {
            return 0;
        }
    }
}

uint8_t strncmp(const uint8_t* str1, const uint8_t* str2, uint64_t len) {
    ASSERT_NOT_NULL(str1);
    ASSERT_NOT_NULL(str2);
    for (uint64_t i = 0;; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (i >= len - 1) {
            return 0;
        }
    }
}

uint32_t strstr(const uint8_t* str1, uint32_t start, const uint8_t* str2) {
    ASSERT_NOT_NULL(str1);
    ASSERT_NOT_NULL(str2);
    uint32_t str1_len = strlen(str1);
    uint32_t str2_len = strlen(str2);
    ASSERT(start < str1_len);
    if (str2_len > str1_len) {
        return -1;
    } else if (str2_len == str1_len) {
        if (0 == strcmp(str1, str2)) {
            return 0;
        }
        return -1;
    }

    for (uint32_t i = start; i < (str1_len - str2_len) + 1; i++) {
        uint8_t ok = 1;
        for (uint32_t j = 0; j < str2_len; j++) {
            if (str2[j] != str1[i + j]) {
                ok = 0;
                break;
            }
        }
        if (ok == 1) {
            return i;
        }
    }
    return -1;
}

uint8_t* substr(const uint8_t* str1, uint32_t start, uint32_t end, uint8_t* str2, uint32_t size) {
    ASSERT_NOT_NULL(str1);
    ASSERT_NOT_NULL(str2);
    ASSERT_NOT_NULL(size);
    uint32_t str1_len = strlen(str1);
    uint32_t str2_len = (end - start);
    ASSERT(start < str1_len);
    ASSERT(end <= str1_len);
    ASSERT(end > start);
    ASSERT((str2_len) <= size);
    for (uint32_t i = 0; i < str2_len; i++) {
        str2[i] = str1[i + start];
    }
    str2[str2_len] = 0;
    return str2;
}
