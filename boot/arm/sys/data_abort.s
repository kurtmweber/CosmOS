#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

.extern c_data_abort_handler
/*
	R14_abt = PC + 8
	SPSR_abt = CPSR
	CPSR[4:0] = 0b10111 //Abort Mode
	CPSR[5] = 0 // ARM state
	CPSR[6] unchanged
	CPSR[7] = 1 // Disable IRQ
	PC = 0x00000010
*/
.global data_abort_handler
data_abort_handler:
	# figure out the PC to go to after the abort
	sub lr, lr, #4

	# store regs on the stack
	push {r0, r1, r2, r3, ip, lr}

	# get the CPSR of the processor when the exception happened
	mrs r0, spsr

	# get the PC of the processor when the exception happened
	mov r1, lr

	# call the C handler
	bl c_data_abort_handler

	# restore the regs and return
	# ^ in this context means restore CPSR from SPSR
	ldm	sp!, {r0, r1, r2, r3, ip, pc}^
