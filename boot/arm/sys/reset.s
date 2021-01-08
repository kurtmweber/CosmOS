#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

.include "sys/defs.s"

.globl reset_handler

/**
	R14_svc = unexpected
	SPSR_svc = unexpected
	CPSR[4:0] = 0b10011 //Supervisor Mode
	CPSR[5] = 0 // ARM state
	CPSR[6] = 1 // Disable FIQ
	CPSR[7] = 1 // Disable IRQ
	PC = 0x00000000
*/
reset_handler:
	# set SVC stack
	msr 		CPSR_c,	#SUPERVISOR_MODE
	ldr			sp, =__svc_stack_top__

	# set USD stack
	msr 		CPSR_c,	#UNDEFINED_MODE
	ldr			sp, =__usd_stack_top__

	# set ABORT stack
	msr 		CPSR_c,	#ABORT_MODE
	ldr			sp, =__abort_stack_top__

	# set IRQ stack
	msr 		CPSR_c,	#IRQ_MODE
	ldr			sp, =__irq_stack_top__

	# set FIQ stack
	msr 		CPSR_c,	#FIQ_MODE
	ldr			sp, =__fiq_stack_top__

	# set Supervisor stack
	msr 		CPSR_c,	#SYSTEM_MODE
	ldr			sp, =__sys_stack_top__

	# copy vector table to address 0
	# see startfuncs.cpp
	bl			copy_vectors

	# zero the bss section
	# see startfuncs.cpp
	bl 			zeroBSS

	# zero the stacks
	# see startfuncs.cpp
	bl			zeroStacks

	# get Program Status Register and enable irqs
	mrs			r0, cpsr
	bic			r0, r0, #IRQ_DISABLE
	bic			r0, r0, #FIQ_DISABLE
	msr			cpsr, r0

	# enter supervisor mode
	msr 		CPSR_c,	#SUPERVISOR_MODE_NO_INTERRUPTS

	# jump to kernel_entrypoint
#	bl			_start
	bl			kernel_entrypoint

	# loop infinitely
	bl			.
