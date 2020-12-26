/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <mm/mm.h>
#include <string/string.h>
#include <panic/assert.h>

uint64_t strlen(const char *s){
	ASSERT_NOT_NULL(s, "s must not be null");
	uint64_t i = 0;
	
	while (s[i]){
		i++;
	}
	
	return i;
}

char *strtrim(const char *s){
	ASSERT_NOT_NULL(s, "s must not be null");
	uint64_t i = 0;
	uint64_t j;
	
	char *tgt;
	
	i = strlen(s);
	
	j = i - 1;
	
	while ((s[j] == ' ') || (s[j] == '\t')){
		j--;
	}
	
	tgt = kmalloc((j + 2) * sizeof(char));		// +1 for the fact that it's a zero-based index, +1 for the terminator
	
	for (i = 0; i <= j; i++){
		tgt[i] = s[i];
	}
	
	tgt[j + 1] = '\0';
	return tgt;
}

char * strcpy(char *dest, const char *src) {
	ASSERT_NOT_NULL(dest, "dest must not be null");
	ASSERT_NOT_NULL(src, "src must not be null");

	uint32_t i;
	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	return dest;
}

char *strcat(char *dest, const char *src) {
	ASSERT_NOT_NULL(dest, "dest must not be null");
	ASSERT_NOT_NULL(src, "src must not be null");

	uint16_t i=0;
	while (dest[i]!=0){
		i++;
	}
	uint16_t j=0;
	while (src[j]!=0){

		dest[i+j]=src[j];
		j++;
	}
	dest[i+j+1]=0;
	return dest;
}

char strcmp(const char *str1, const char *str2) {
	ASSERT_NOT_NULL(str1, "str1 must not be null");
	ASSERT_NOT_NULL(str2, "str2 must not be null");

	for (int i = 0; ; i++) {
        if (str1[i] != str2[i]){
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (str1[i] == '\0'){
            return 0;
        }
    }
}

// https://wiki.osdev.org/Meaty_Skeleton#libc.2Fstring.2Fmemcpy.c
void* memcpy(void* restrict dstptr, const void* restrict srcptr, uint64_t size) {
	ASSERT_NOT_NULL(dstptr, "dstptr must not be null");
	ASSERT_NOT_NULL(srcptr, "srcptr must not be null");

	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (uint64_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}



