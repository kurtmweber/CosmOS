/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>

__attribute__ ((interrupt)) void irq0(stackFrame *frame){	
	route_interrupt(0, frame);
	pic_send_eoi(0);
}

__attribute__ ((interrupt)) void irq1(stackFrame *frame){
	route_interrupt(1, frame);
	pic_send_eoi(1);
}

__attribute__ ((interrupt)) void irq2(stackFrame *frame){
	route_interrupt(2, frame);
	pic_send_eoi(2);
}

__attribute__ ((interrupt)) void irq3(stackFrame *frame){
	route_interrupt(3, frame);
	pic_send_eoi(3);
}

__attribute__ ((interrupt)) void irq4(stackFrame *frame){
	route_interrupt(4, frame);
	pic_send_eoi(4);
}

__attribute__ ((interrupt)) void irq5(stackFrame *frame){
	route_interrupt(5, frame);
	pic_send_eoi(5);
}

__attribute__ ((interrupt)) void irq6(stackFrame *frame){
	route_interrupt(6, frame);
	pic_send_eoi(6);
}

__attribute__ ((interrupt)) void irq7(stackFrame *frame){
	route_interrupt(7, frame);
	pic_send_eoi(7);
}

__attribute__ ((interrupt)) void irq8(stackFrame *frame){
	route_interrupt(8, frame);
	pic_send_eoi(8);
}

__attribute__ ((interrupt)) void irq9(stackFrame *frame){
	route_interrupt(9, frame);
	pic_send_eoi(9);
}

__attribute__ ((interrupt)) void irq10(stackFrame *frame){
	route_interrupt(10, frame);
	pic_send_eoi(10);
}

__attribute__ ((interrupt)) void irq11(stackFrame *frame){
	route_interrupt(11, frame);
	pic_send_eoi(11);
}

__attribute__ ((interrupt)) void irq12(stackFrame *frame){
	route_interrupt(12, frame);
	pic_send_eoi(12);
}

__attribute__ ((interrupt)) void irq13(stackFrame *frame){
	route_interrupt(13, frame);
	pic_send_eoi(13);
}

__attribute__ ((interrupt)) void irq14(stackFrame *frame){
	route_interrupt(14, frame);
	pic_send_eoi(14);
}

__attribute__ ((interrupt)) void irq15(stackFrame *frame){
	route_interrupt(15, frame);
	pic_send_eoi(15);
}
