//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/kernelstring/kernelstring.h>
#include <mm/mm.h>
#include <panic/panic.h>

struct kernelstring* stringNew() {
    struct kernelstring* ret = (struct kernelstring*) kmalloc(sizeof(kernelstring_t));
    ret->str = 0;
    ret->length=0;
}

void stringDelete(struct kernelstring* str) {
    if (0!=str){
        if (0!=str->str) {
            kfree(str->str);
        }
        kfree(str);
    } else {
        panic("null kernelstring\n");
    }
}

uint16_t stringLength(struct kernelstring* str) {
    if (0!=str){
        return str->length;
    } else {
        panic("null kernelstring\n");
    }
}


