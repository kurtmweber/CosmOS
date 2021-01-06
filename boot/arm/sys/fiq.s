#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

.extern c_fiq_handler
/*
	R14_abt = PC + 4
	SPSR_abt = CPSR
	CPSR[4:0] = 0b10010 //IRQ Mode
	CPSR[5] = 0 // ARM state
	CPSR[6] = 1 //Disable FIQ
	CPSR[7] = 1 // Disable IRQ
	PC = 0x0000001C
*/

.global fiq_handler
fiq_handler:
	# figure out the PC to go to after the FIQ
	sub lr, lr, #4

	# store regs on the FIQ stack
	stmfd sp!, {r0-r12,lr}

	# call the C FIQ handler
	bl c_fiq_handler

	# restore the regs and return
	# ^ in this context means restore CPSR from SPSR
	ldmfd sp!, {r0-r12,pc}^
