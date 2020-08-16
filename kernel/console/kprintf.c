/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KPRINTF_C
#define _KPRINTF_C

#include <types.h>
#include <console/console.h>
#include <console/drivers/drivers.h>
#include <debug_error/debug_error.h>

uint64_t kprintf(const char *s, ...){
	__builtin_va_list ap;
	uint64_t idx_in = 0, idx_out = 0;
	uint64_t chars_written = 0;
	char out_str[64] = { '\0' };
	
	while (s[idx_in]){
		if (s[idx_in] != '%'){
			out_str[idx_out] = s[idx_in];
			idx_out++;
		} else {
			switch(s[++idx_in]){
				case '%':
					out_str[idx_out] = '%';
					idx_out++;
					break;
				default:
					break;
			}
		}
		
		idx_in++;
		
		if (idx_out == 63){
			out_str[63] = '\0';
			console_write(out_str);
			idx_out = 0;
			chars_written += 63;
		}
	}
	
	chars_written += idx_out;
	out_str[idx_out] = '\0';
	console_write(out_str);
	
	return chars_written;
}

#endif