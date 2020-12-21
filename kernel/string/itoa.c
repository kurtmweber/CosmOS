/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <string/string.h>
#include <panic/panic.h>

const uint8_t c_array[] = "0123456789ABCDEF";

void uitoa3(uint64_t n, uint8_t *s, uint16_t len, uint8_t base){
	/*
	* sanity check
	*/
	if ((base <2)|| (base>16)){
		panic("Invalid base passed to uitoa3");
	}
	/*
	* sanity check
	*/
	if (0==s){
		panic("Invalid string pointer passed to uitoa3");
	}
	/*
	* total count of digits
	*/
	uint16_t i = 0;
	uint8_t *rev = s;

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

	uint8_t tmp;
	while (rev < s){
		tmp = *rev;
		*rev = *s;
		*s = tmp;
		s--;
		rev++;
	}
}
