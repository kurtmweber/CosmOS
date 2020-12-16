
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _INTERRUPT_ROUTER_C
#define _INTERRUPT_ROUTER_C

#include <interrupts/interrupt_router.h>

#define NUMBER_INTERRUPTS 16

interruptHandler interruptHandlers[NUMBER_INTERRUPTS];

void interrupt_router_init() {
    for (int i=0; i<NUMBER_INTERRUPTS;i++){
        interruptHandlers[i]=0;
    }    
}

void registerInterruptHandler(int interruptNumber, interruptHandler func){
    if ((interruptNumber>=0) && (interruptNumber<NUMBER_INTERRUPTS)){
        interruptHandlers[interruptNumber]=func;
    }
}

interruptHandler getInterruptHandler(int interruptNumber){
    if ((interruptNumber>=0) && (interruptNumber<NUMBER_INTERRUPTS)){
        return interruptHandlers[interruptNumber];
    }
    return 0;
}

void routeInterrupt(int interruptNumber, stackFrame *frame){
    if ((interruptNumber>=0) && (interruptNumber<NUMBER_INTERRUPTS)){
        interruptHandler handler = getInterruptHandler(interruptNumber);
        if (handler!=0){
            (*handler)();
        }
    }
}

#endif
