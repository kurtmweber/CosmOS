/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/debug/assert.h>
#include <sys/i386/interrupts/exceptions.h>

__attribute__ ((interrupt)) void isrPFE(stackFrame *frame, uint64_t error){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	panic("Page fault error!");
}
