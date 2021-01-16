/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IRQ_H
#define _IRQ_H

#include <types.h>

typedef struct stackFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t fflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) stackFrame;

void irq0_handler(stackFrame *frame);
void irq1_handler(stackFrame *frame);
void irq2_handler(stackFrame *frame);
void irq3_handler(stackFrame *frame);
void irq4_handler(stackFrame *frame);
void irq5_handler(stackFrame *frame);
void irq6_handler(stackFrame *frame);
void irq7_handler(stackFrame *frame);
void irq8_handler(stackFrame *frame);
void irq9_handler(stackFrame *frame);
void irq10_handler(stackFrame *frame);
void irq11_handler(stackFrame *frame);
void irq12_handler(stackFrame *frame);
void irq13_handler(stackFrame *frame);
void irq14_handler(stackFrame *frame);
void irq15_handler(stackFrame *frame);

#endif
