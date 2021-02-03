/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <types.h>

void uitoa3(uint64_t n, char* s, uint16_t len, uint8_t base) {
    ASSERT_NOT_NULL(s);
    ASSERT(((base > 0) && (base <= 16)));

    uint16_t i = 0;
    char* rev = s;
    char tmp;

    char c_array[] = "0123456789ABCDEF";

    do {
        *s = c_array[n % base];
        n /= base;
        s++;
        i++;
    } while ((n) && (i < len));

    *s = '\0';

    // decrement s because otherwise it will point to the terminating \0
    // which will mean that the reversed string that we build next will begin with \0
    s--;

    while (rev < s) {
        tmp = *rev;
        *rev = *s;
        *s = tmp;
        s--;
        rev++;
    }

    return;
}
