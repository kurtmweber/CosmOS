/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DEBUGINT_C
#define _DEBUGINT_C

#include <types.h>
#include <console/drivers/drivers.h>
#include <string/string.h>

void debug_print_uint64(uint64_t n){
	char s[UINT64_DECIMAL_STRING_LENGTH];
	
	uitoa3(n, s, UINT64_DECIMAL_STRING_LENGTH);
	
	console_write(s);
	console_write("\n");
	
	return;
	
}

#endif