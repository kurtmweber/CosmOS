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

uint32_t stringLength(struct kernelstring* str) {
    if (0!=str){
        return str->length;
    } else {
        panic("null kernelstring\n");
    }
}

struct kernelstring* stringFromCStr(const int8_t* str) {
    struct kernelstring* ret = stringNew();
    /*
    * get length
    */
	for (ret->length = 0; str[ret->length] != '\0'; ret->length++);
    /*
    * allocate
    */
    ret->str = (uint8_t*) kmalloc(ret->length+1);
    /*
    * copy
    */
    for (ret->length = 0; str[ret->length] != '\0'; ret->length++) {
        ret->str[ret->length]=str[ret->length];
    }
    /*
    * null
    */
    ret->str[ret->length]=0;
}

const int8_t* stringGetCStr(struct kernelstring* str) {
    if (0!=str){
        return str->str;
    } else {
        panic("null kernelstring\n");
    }
}




