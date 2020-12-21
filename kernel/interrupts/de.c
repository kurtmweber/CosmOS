/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <panic/panic.h>
#include <interrupts/interrupts.h>

__attribute__ ((interrupt)) void isrDE(stackFrame *frame){
	panic("Division by zero!");
}
