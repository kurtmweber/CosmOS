/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

// For memset()/memcpy()/memmove()-family functions

#include <types.h>

void *memset(void *s, uint8_t c, size_t n){
    size_t i;

    for (i = 0; i < n; i++){
        *(uint8_t *)s = c;
    }

    return s;
}