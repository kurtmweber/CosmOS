//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/ringbuffer/ringbuffer.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/string/string.h>
#include <tests/sys/test_ringbuffer.h>

char R_A[] = {"A"};
char R_B[] = {"BB"};
char R_C[] = {"CCC"};
char R_D[] = {"DDDD"};
char R_E[] = {"EEEEE"};
char R_F[] = {"FFFFFF"};

void test_ringbuffer() {
    kprintf("Testing ringbuffer\n");

    struct ringbuffer* rb = ringbuffer_new(4);
    ASSERT(rb->size == 4);
    ASSERT(ringbuffer_size(rb) == 4);

    /*
     * add and remove 1
     */
    ringbuffer_add(rb, R_A);
    char* p = ringbuffer_consume(rb);
    ASSERT(strcmp(p, R_A) == 0);
    /*
     * add and remove 3
     */
    ringbuffer_add(rb, R_B);
    ringbuffer_add(rb, R_C);
    ringbuffer_add(rb, R_D);
    p = ringbuffer_consume(rb);
    ASSERT(strcmp(p, R_B) == 0);
    p = ringbuffer_consume(rb);
    ASSERT(strcmp(p, R_C) == 0);
    p = ringbuffer_consume(rb);
    ASSERT(strcmp(p, R_D) == 0);

    ringbuffer_delete(rb);
}
