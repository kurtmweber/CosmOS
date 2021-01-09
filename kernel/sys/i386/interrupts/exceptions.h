/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DE_H
#define _DE_H


#include <types.h>
#include <sys/debug/assert.h>
#include <sys/i386/interrupts/irq.h>

void isrDE(stackFrame *frame);
void isrPFE(stackFrame *frame, uint64_t error);

#endif