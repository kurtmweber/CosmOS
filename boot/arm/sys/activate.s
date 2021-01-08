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

#
# activate is called by the scheduler to switch from supervisor mode to a user mode process
#
# r0 contains address of unit32_t to get new stack pointer from
#
# The saved kernel process stack looks like this:
#
# r4-r10
# fp
# ip (contains the kernel stack pointer sp)
# lr
#
# The saved user process stack looks like this:
#
# spsr  <--- stack pointer
# pc
# r0-r10
# fp
# ip
# lr
#
# extern "C" uint32_t *activate(uint32_t *stack);
#

.global activate
activate:
	# we start in supervisor mode, no interrupts
	# save kernel state onto the kernel stack (we're putting the kernel stack pointer into the inter-process register)
	mov ip, sp
	push {r1-r10,fp,ip,lr}

	# get the user process spsr (into ip) and next instruction (into lr) from the user stack
	ldmfd r0!, {ip,lr}

	# set the spsr
	msr SPSR, ip

	# switch to system mode.
	msr CPSR_c, #SYSTEM_MODE_NO_INTERRUPTS

	# set the stack pointer. note that system mode uses the user-mode registers, so we are setting the user mode sp here
	mov sp, r0

	# pop the user process registers.  again, we are in system mode here, so we are popping the user-mode regs.
	# we have set the SP to the user processes stack in the previous instruction
	pop {r0-r10,fp,ip,lr}

	# back to supervisor mode (this function was called by the scheduler, in supervisor mode)
	msr CPSR_c, #SUPERVISOR_MODE_NO_INTERRUPTS

	# next user-mode instruction
	movs pc, lr

#
# deactivate is called by the irq handler (in IRQ mode) and switches back to the kernel process from a user process
#
# deactivate stores the current user process and then returns to the kernel process
#
# extern "C" uint32_t *deactivate();
#

.global deactivate
deactivate:
	# we start in IRQ mode, no interrupts
	# switch to system mode
	msr CPSR_c, #SYSTEM_MODE_NO_INTERRUPTS

	# save user state (we are in system mode; the user SP is the system SP)
	push {r0-r10,fp,ip,lr}

	# put the user stack pointer into r0
	mov r0, sp

	# IRQ mode
	msr CPSR_c, #IRQ_MODE_NO_INTERRUPTS

	# put user mode spsr into ip
	mrs ip, SPSR

	# store spsr (which is in ip) and next instruction (which is in lr) onto the user stack
	stmfd r0!, {ip,lr}

	# supervisor mode (the mode the kernel runs in)
	msr CPSR_c, #SUPERVISOR_MODE_NO_INTERRUPTS

	# Load kernel state (note that the kernel sp is in ip)
	pop {r1-r10,fp,ip,lr}
	mov sp, ip

	# next kernel instruction
	bx lr
