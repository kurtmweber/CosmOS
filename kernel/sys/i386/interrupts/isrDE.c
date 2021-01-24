/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/i386/interrupts/exceptions.h>
#include <types.h>

void isrDE_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    panic("Division by zero!");
}
