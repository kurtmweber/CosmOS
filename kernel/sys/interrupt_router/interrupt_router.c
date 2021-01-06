//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/debug/assert.h>
#include <sys/collection/array/array.h>
#include <sys/collection/list/list.h>

#define NUMBER_INTERRUPTS 16

/**
* array of lists
*/ 
struct array* interruptHandlers;

/**
* create the array of interrupts and then add a list for each entry
*/
void interrupt_router_init() {
    interruptHandlers = array_new(NUMBER_INTERRUPTS);
    for (int i=0; i<NUMBER_INTERRUPTS;i++){
        array_set(interruptHandlers,i, list_new());
    }    
}

/**
* register an interrupt handler callback
*/
void interrupt_router_register_interrupt_handler(int interruptNumber, interruptHandler func){
	ASSERT_NOT_NULL(func, "func cannot be null");
	ASSERT_NOT_NULL(interruptHandlers, "interruptHandlers cannot be null.  Has interrupt_router been initialized?");

    if ((interruptNumber>=0) && (interruptNumber<NUMBER_INTERRUPTS)){
        struct list* lst = array_get(interruptHandlers,interruptNumber);
        if (0==lst){
            panic("List should not be null");
        }
        list_add(lst, func);
    } else {
        panic("Invalid interrupt number");
    }
}

/**
* route an interrupt
*/
void interrupt_router_route_interrupt(int interruptNumber, stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	ASSERT_NOT_NULL(interruptHandlers, "interruptHandlers cannot be null.  Has interrupt_router been initialized?");

    if ((interruptNumber>=0) && (interruptNumber<NUMBER_INTERRUPTS)){
        struct list* lst = array_get(interruptHandlers,interruptNumber);
        if (0==lst){
            panic("List should not be null");
        }
        for (uint16_t i=0; i<list_count(lst);i++){
            interruptHandler handler = (interruptHandler) list_get(lst,i);
            if (0!=handler){
                (*handler)(frame);
            } else {
                panic("Interrupt handler should not be null");
            }
        }
    } else {
        panic("Invalid interrupt number");
    }
}

