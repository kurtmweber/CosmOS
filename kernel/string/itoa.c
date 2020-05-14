/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ITOA_C
#define _ITOA_C

#include <types.h>

void uitoa3(uint64_t n, char *s, uint8_t len){
	// use this function before mmu is set up or, maybe
	// when debugging mmu
	
	uint8_t i = 0;
	char *rev = s;
	char tmp;
	
	do {
		*s = (n % 10) + '0';
		n /= 10;
		s++;
		i++;
	} while ((n) && (i < len));
	
	*s = '\0';
	
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
}

#endif