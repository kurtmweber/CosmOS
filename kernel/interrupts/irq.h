/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IRQ_H
#define _IRQ_H

#include <types.h>

typedef struct stackFrame{
	uint64_t rip;
	uint64_t cs;
	uint64_t fflags;
	uint64_t rsp;
	uint64_t ss;
	}__attribute__((packed)) stackFrame;

void irq0(stackFrame *frame);
void irq1(stackFrame *frame);
void irq2(stackFrame *frame);
void irq3(stackFrame *frame);
void irq4(stackFrame *frame);
void irq5(stackFrame *frame);
void irq6(stackFrame *frame);
void irq7(stackFrame *frame);
void irq8(stackFrame *frame);
void irq9(stackFrame *frame);
void irq10(stackFrame *frame);
void irq11(stackFrame *frame);
void irq12(stackFrame *frame);
void irq13(stackFrame *frame);
void irq14(stackFrame *frame);
void irq15(stackFrame *frame);

#endif
