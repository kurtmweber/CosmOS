//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/string/string.h>
#include <mm/mm.h>
#include <panic/panic.h>

#define DEFAULT_STRING_SIZE 32

struct string* stringNew() {
    struct string* ret = (struct string*) kmalloc(sizeof(string_t));
    ret->arr = arrayNew(DEFAULT_STRING_SIZE);
    ret->length=0;
}

void stringDelete(struct string* str) {
    if (0!=str){
        if (0==str->arr){
            panic("why is the underlying array null?!");
        }
        arrayDelete(str->arr);
        kfree(str);
    } else {
        panic("null string\n");
    }
}


