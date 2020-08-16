/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _STRING_C
#define _STRING_C

#include <types.h>

uint64_t strlen(const char *s){
	uint64_t i = 0;
	
	while (s[i]){
		i++;
	}
	
	return i;
}

#endif