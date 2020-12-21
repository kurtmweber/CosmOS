/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <mm/mm.h>
#include <string/string.h>

uint64_t strlen(const char *s){
	uint64_t i = 0;
	
	while (s[i]){
		i++;
	}
	
	return i;
}

char *strtrim(const char *s){
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
	uint32_t i;
	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	return dest;
}

