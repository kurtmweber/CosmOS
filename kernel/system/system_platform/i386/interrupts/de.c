/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <debug/assert.h>
#include <system/system_platform/i386/interrupts/de.h>

__attribute__ ((interrupt)) void isrDE(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	panic("Division by zero!");
}
