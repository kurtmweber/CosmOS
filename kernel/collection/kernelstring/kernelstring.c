//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/kernelstring/kernelstring.h>
#include <mm/mm.h>
#include <panic/panic.h>
#include <console/console.h>

struct kernelstring* stringNew() {
    struct kernelstring* ret = (struct kernelstring*) kmalloc(sizeof(struct kernelstring));
    ret->str = 0;
    ret->length=0;
    return ret;
}

struct kernelstring* stringNewSized(uint32_t size) {
    if (size>0) {
        struct kernelstring* ret = (struct kernelstring*) kmalloc(sizeof(struct kernelstring));
        ret->str = kmalloc(size+1);
        ret->length = 0;
        for (uint32_t i=0; i<size;i++){
            ret->str[i]=0;
        }
        return ret;
    } else {
        panic("invalid string size\n");
    }
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
    /*
    * get length
    */
    uint32_t len=0;
	for (len = 0; str[len] != 0; len++);

    /*
    * allocate
    */
    struct kernelstring* ret = stringNewSized(len);

    /*
    * copy
    */
   uint32_t i=0;
    for (i = 0; str[i] != 0; i++) {
        ret->str[i]=str[i];
    }
    ret->length=len;
    /*
    * null
    */
    ret->str[i+1]=0;
    /*
    * done
    */
   return ret;
}

const int8_t* stringGetCStr(struct kernelstring* str) {
    if (0!=str){
        return str->str;
    } else {
        panic("null kernelstring\n");
    }
}

/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

struct kernelstring*  stringItoa3(uint64_t n, uint8_t base) {
    /*
    * valid bases
    */
   if ((base>0)&&(base <=16)) {
        /*
        * max four binary digits per decimal digits, so just use 5
        */
        const uint32_t len = n*5;
        struct kernelstring* ret = stringNewSized(len);
        uint8_t i = 0;
        uint8_t *s = ret->str;
        uint8_t *rev = s;
        char tmp;
        char c_array[] = "0123456789ABCDEF";
        
        do {
            *s = c_array[n % base];
            n /= base;
            s++;
            i++;
        } while ((n) && (i < len));
        
        *s = '\0';
        ret->length=s-(ret->str);
        
        // decrement s because otherwise it will point to the terminating \0
        // which will mean that the reversed string that we build next will begin with \0
        s--;
        while (rev < s){
            tmp = *rev;
            *rev = *s;
            *s = tmp;
            s--;
            rev++;
        }
        return ret;
    } else {
        panic("illegal base\n");
    }
}

struct kernelstring*  stringConcat(struct kernelstring* str1, struct kernelstring* str2) {
    if ((0!=str1) && (0!=str2)){
        uint32_t len = str1->length + str2->length;
        struct kernelstring* ret = stringNewSized(len);
        for (uint32_t i=0; i<str1->length;i++){
            ret->str[i]=str1->str[i];
        }
        for (uint32_t j=0; j<str2->length;j++){
            ret->str[j+str1->length]=str2->str[j];
        }
        ret->length = len;
        return ret;
    } else {
        panic("null kernelstring\n");
    }
}

struct kernelstring*  stringCopy(struct kernelstring* str) {
    if (0!=str){
        struct kernelstring* ret =  stringNewSized(str->length);
        uint32_t i=0;
        for (i=0; i<str->length;i++){
        }
        ret->str[i+1]=0;
        ret->length = str->length;
        return ret;
    } else {
        panic("null kernelstring\n");
    }
}




