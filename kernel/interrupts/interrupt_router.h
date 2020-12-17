//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _INTERRUPT_ROUTER_H
#define _INTERRUPT_ROUTER_H

#include <interrupts/interrupts.h>

/**
 * interrupt routing to be used by device drivers
 */
typedef void (*interruptHandler)(stackFrame *frame);
void interrupt_router_init();
void registerInterruptHandler(int interruptNumber, interruptHandler func);

/*
* called by ISR in irq.c
*/
void routeInterrupt(int interruptNumber, stackFrame *frame);

#endif