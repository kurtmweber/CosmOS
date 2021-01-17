//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>

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
    for (int i = 0; i < NUMBER_INTERRUPTS; i++) {
        array_set(interruptHandlers, i, arraylist_new());
    }
}

/**
 * register an interrupt handler callback
 */
void interrupt_router_register_interrupt_handler(int interruptNumber, interruptHandler func) {
    ASSERT_NOT_NULL(func);
    ASSERT_NOT_NULL(interruptHandlers);

    if ((interruptNumber >= 0) && (interruptNumber < NUMBER_INTERRUPTS)) {
        struct arraylist* lst = array_get(interruptHandlers, interruptNumber);
        if (0 == lst) {
            panic("List should not be null");
        }
        arraylist_add(lst, func);
    } else {
        panic("Invalid interrupt number");
    }
}

/**
 * route an interrupt
 */
void interrupt_router_route_interrupt(int interruptNumber, stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    ASSERT_NOT_NULL(interruptHandlers);

    if ((interruptNumber >= 0) && (interruptNumber < NUMBER_INTERRUPTS)) {
        struct arraylist* lst = array_get(interruptHandlers, interruptNumber);
        if (0 == lst) {
            panic("List should not be null");
        }
        for (uint16_t i = 0; i < arraylist_count(lst); i++) {
            interruptHandler handler = (interruptHandler)arraylist_get(lst, i);
            if (0 != handler) {
                (*handler)(frame);
            } else {
                panic("Interrupt handler should not be null");
            }
        }
    } else {
        panic("Invalid interrupt number");
    }
}
