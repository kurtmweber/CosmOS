/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>

__attribute__ ((interrupt)) void irq0(stackFrame *frame){	
	routeInterrupt(0, frame);
	pic_send_eoi(0);
}

__attribute__ ((interrupt)) void irq1(stackFrame *frame){
	routeInterrupt(1, frame);
	pic_send_eoi(1);
}

__attribute__ ((interrupt)) void irq2(stackFrame *frame){
	routeInterrupt(2, frame);
	pic_send_eoi(2);
}

__attribute__ ((interrupt)) void irq3(stackFrame *frame){
	routeInterrupt(3, frame);
	pic_send_eoi(3);
}

__attribute__ ((interrupt)) void irq4(stackFrame *frame){
	routeInterrupt(4, frame);
	pic_send_eoi(4);
}

__attribute__ ((interrupt)) void irq5(stackFrame *frame){
	routeInterrupt(5, frame);
	pic_send_eoi(5);
}

__attribute__ ((interrupt)) void irq6(stackFrame *frame){
	routeInterrupt(6, frame);
	pic_send_eoi(6);
}

__attribute__ ((interrupt)) void irq7(stackFrame *frame){
	routeInterrupt(7, frame);
	pic_send_eoi(7);
}

__attribute__ ((interrupt)) void irq8(stackFrame *frame){
	routeInterrupt(8, frame);
	pic_send_eoi(8);
}

__attribute__ ((interrupt)) void irq9(stackFrame *frame){
	routeInterrupt(9, frame);
	pic_send_eoi(9);
}

__attribute__ ((interrupt)) void irq10(stackFrame *frame){
	routeInterrupt(10, frame);
	pic_send_eoi(10);
}

__attribute__ ((interrupt)) void irq11(stackFrame *frame){
	routeInterrupt(11, frame);
	pic_send_eoi(11);
}

__attribute__ ((interrupt)) void irq12(stackFrame *frame){
	routeInterrupt(12, frame);
	pic_send_eoi(12);
}

__attribute__ ((interrupt)) void irq13(stackFrame *frame){
	routeInterrupt(13, frame);
	pic_send_eoi(13);
}

__attribute__ ((interrupt)) void irq14(stackFrame *frame){
	routeInterrupt(14, frame);
	pic_send_eoi(14);
}

__attribute__ ((interrupt)) void irq15(stackFrame *frame){
	routeInterrupt(15, frame);
	pic_send_eoi(15);
}
