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
#include <string/string.h>

uint64_t kprintf(const char *s, ...){
	__builtin_va_list ap;
	uint64_t idx_in = 0, idx_out = 0;
	uint64_t chars_written = 0;
	char out_str[64] = { '\0' };
	
	__builtin_va_start(ap, s);
	
	while (s[idx_in]){
		if (s[idx_in] != '%'){
			out_str[idx_out] = s[idx_in];
			idx_out++;
		} else {
			switch(s[idx_in + 1]){
				case '%':
					out_str[idx_out] = '%';
					idx_out++;
					idx_in++;
					break;
				default:
					chars_written += idx_out;
					out_str[idx_out] = '\0';
					idx_out = 0;
					console_write(out_str);
					
					idx_in += kprintf_proc_format_string(&s[idx_in + 1], &chars_written, ap);
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
	
	__builtin_va_end(ap);
	
	chars_written += idx_out;
	out_str[idx_out] = '\0';
	console_write(out_str);
	
	return chars_written;
}

uint64_t kprintf_proc_format_string(const char *s, uint64_t *chars_written, __builtin_va_list ap){
	// maximum length of an output string is 66 bytes:
	// 64 binary digits (for an unsigned 64-bit type) or 63 and a +/- sign (if signed)
	// possible "b" sigil at end
	// NULL terminator
	char output[66];
	uint64_t i = 0;
	uint64_t consumed = 0;
	uint8_t width = 0;
	
	// conversion variables
	unsigned int conv_uint;
	unsigned long conv_ulong;
	
	while (s[i]){
		consumed++;
		switch(s[i++]){
			case 'h':
				width = 8;
				break;
			case 'l':
				if (width == 32){
					width = 64;
				} else {
					width = 32;
				}
				break;
			case 'u':
				if (!width){
					width = 16;
				}
				switch(width){
					// fallthrough is intentional, as 8-bit and 16-bit unsigned ints are promoted by compiler to 32-bit unsigned int
					case 8:
					case 16:
					case 32:
						conv_uint = __builtin_va_arg(ap, unsigned int);
						uitoa3(conv_uint, output, 66);
						break;
					case 64:
						conv_ulong = __builtin_va_arg(ap, unsigned long);
						uitoa3(conv_ulong, output, 66);
						break;
				}
				console_write(output);
				*chars_written += strlen(output);
				return consumed;
			default:
				return consumed;
		}
	}
}

#endif