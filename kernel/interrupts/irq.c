/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IRQ_C
#define _IRQ_C

#include <types.h>
#include <console/console.h>
#include <keyboard/keyboard.h>
#include <interrupts/interrupts.h>
#include <timing/timing.h>

__attribute__ ((interrupt)) void irq0(stackFrame *frame){
	pic_send_eoi(0);
}

__attribute__ ((interrupt)) void irq1(stackFrame *frame){
	keyboard_irq_read();
	pic_send_eoi(1);
}

__attribute__ ((interrupt)) void irq2(stackFrame *frame){
	pic_send_eoi(2);
}

__attribute__ ((interrupt)) void irq3(stackFrame *frame){
	pic_send_eoi(3);
}

__attribute__ ((interrupt)) void irq4(stackFrame *frame){
	pic_send_eoi(4);
}

__attribute__ ((interrupt)) void irq5(stackFrame *frame){
	pic_send_eoi(5);
}

__attribute__ ((interrupt)) void irq6(stackFrame *frame){
	pic_send_eoi(6);
}

__attribute__ ((interrupt)) void irq7(stackFrame *frame){
	pic_send_eoi(7);
}

__attribute__ ((interrupt)) void irq8(stackFrame *frame){
	rtc_handle_irq();
	pic_send_eoi(8);
}

__attribute__ ((interrupt)) void irq9(stackFrame *frame){
	pic_send_eoi(9);
}

__attribute__ ((interrupt)) void irq10(stackFrame *frame){
	pic_send_eoi(10);
}

__attribute__ ((interrupt)) void irq11(stackFrame *frame){
	pic_send_eoi(11);
}

__attribute__ ((interrupt)) void irq12(stackFrame *frame){
	pic_send_eoi(12);
}

__attribute__ ((interrupt)) void irq13(stackFrame *frame){
	pic_send_eoi(13);
}

__attribute__ ((interrupt)) void irq14(stackFrame *frame){
	pic_send_eoi(14);
}

__attribute__ ((interrupt)) void irq15(stackFrame *frame){
	pic_send_eoi(15);
}

#endif