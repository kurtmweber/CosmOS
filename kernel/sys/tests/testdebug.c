//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/tests/testdebug.h>
#include <sys/debug/debug.h>

void test_debug() {
	uint8_t testdata[] = {0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x11, 0x13, 0x15, 0x17, 0x19, 0x1B };
	debug_show_memblock(testdata,10);
}

