/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/
#ifndef _DE_C
#define _DE_C

#include <types.h>
#include <debug_error/debug_error.h>
#include <interrupts/interrupts.h>

__attribute__ ((interrupt)) void isrDE(stackFrame *frame){
	panic("Division by zero!");
}

#endif