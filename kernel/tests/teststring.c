//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/teststring.h>
#include <string/string.h>
#include <console/console.h>
#include <collection/kernelstring/kernelstring.h>

void stringtest() {
	// concat
	struct kernelstring* string1 = string_from_cstr("hello");
	struct kernelstring* string2 = string_from_cstr(" world\n");

	kprintf("%llu\n",string1->length);
	kprintf(string_get_cstr(string1));
	kprintf("%llu\n",string2->length);
	kprintf(string_get_cstr(string2));

	struct kernelstring* string3 = string_concat(string1, string2);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));

	struct kernelstring* string4 = string_itoa3(1000, 16);
	kprintf("%llu\n",string4->length);
	kprintf(string_get_cstr(string4));

	struct kernelstring* string5 = string_copy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));
	kprintf("\n");

	struct kernelstring* string6 = string_copy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));
	kprintf("\n");

	struct kernelstring* string7 = string_from_cstr("beer           ");
	kprintf("%llu\n",string7->length);
	kprintf(string_get_cstr(string7));
	kprintf("\n");

	struct kernelstring* string8 = string_trim(string7);
	kprintf("%llu\n",string8->length);
	kprintf(string_get_cstr(string8));
	kprintf("\n");
}