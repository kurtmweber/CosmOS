/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/asm/misc.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/interrupts/exceptions.h>
#include <sys/i386/mm/mm.h>

__attribute__ ((interrupt)) void isrPFE(stackFrame *frame, uint64_t error){
	ASSERT_NOT_NULL(frame);

    page_fault_handler(error, asm_cr2_read(), asm_cr3_read());

    return;
}
