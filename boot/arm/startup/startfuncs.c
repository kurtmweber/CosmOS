/**
 * Copyright (C) khubla.com - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Tom Everett <tom@khubla.com>, 2014-2017
 */

#include <sys/types.h>

/**
 * this is called from reset.s
 * copy the vectors to address 0 in RAM
 * vectors_start and vectors_end are defined in vector.s
 */
void copy_vectors(void) {
    extern uint32_t __vectors_start__;
    extern uint32_t __vectors_end__;
    uint32_t* vectors_start = &__vectors_start__;
    uint32_t* vectors_end = &__vectors_end__;
    uint32_t* vectors_dst = (uint32_t*)0;

    if (vectors_start > 0) {
        while (vectors_start < vectors_end)
            *vectors_dst++ = *vectors_start++;
    }
}

/**
 * write zeros to an zone of memory
 */
void writeZeros(uint32_t* start, uint32_t* end) {
    uint32_t* src = start;
    while (src < end) {
        *src = 0;
        src++;
    }
}

/**
 * this is called from reset.s
 * __bss_start__ and __bss_end__ are defined in link-arm-eabi.ld
 */
/**
 * zero the bss section
 */
void zeroBSS(void) {
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;
    writeZeros(&__bss_start__, &__bss_end__);
}

/**
 * zero the stacks
 */
void zeroStacks(void) {
    extern uint32_t __sys_stack_top__;
    extern uint32_t __irq_stack_top__;
    extern uint32_t __fiq_stack_top__;
    extern uint32_t __svc_stack_top__;
    extern uint32_t __abort_stack_top__;
    extern uint32_t __usd_stack_top__;
    extern uint32_t SYS_STACK_SIZE;
    extern uint32_t IRQ_STACK_SIZE;
    extern uint32_t FIQ_STACK_SIZE;
    extern uint32_t SVC_STACK_SIZE;
    extern uint32_t ABORT_STACK_SIZE;
    extern uint32_t USB_STACK_SIZE;

    //	writeZeros(&__sys_stack_top__ - SYS_STACK_SIZE, &__sys_stack_top__);
    //	writeZeros(&__irq_stack_top__ - IRQ_STACK_SIZE, &__irq_stack_top__);
    //	writeZeros(&__fiq_stack_top__ - FIQ_STACK_SIZE, &__fiq_stack_top__);
    //	writeZeros(&__svc_stack_top__ - SVC_STACK_SIZE, &__svc_stack_top__);
    //	writeZeros(&__abort_stack_top__ - ABORT_STACK_SIZE, &__abort_stack_top__);
    //	writeZeros(&__usd_stack_top__ - USB_STACK_SIZE, &__usd_stack_top__);
}
