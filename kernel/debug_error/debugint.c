/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DEBUGINT_C
#define _DEBUGINT_C

#include <types.h>
#include <console/console.h>
#include <string/string.h>

void debug_print_uint64(uint64_t n){
	char s[UINT64_DECIMAL_STRING_LENGTH];
	
	uitoa3(n, s, UINT64_DECIMAL_STRING_LENGTH);
	
	console_write_line(s);
	
	return;
	
}

#endif