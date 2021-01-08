#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

.extern contextswitch
.extern c_irq_handler

.include "sys/defs.s"
/*
	R14_abt = PC+4
	SPSR_abt = CPSR
	CPSR[4:0] = 0b10010 //Abort Mode
	CPSR[5] = 0
	CPSR[6] unchanged
	CPSR[7] = 1 // Disable IRQ
	PC = 0x0000018
*/

.global irq_handler
irq_handler:
	# figure out the PC to go to after the IRQ
	sub lr, lr, #4

	# store regs on the IRQ stack
	stmfd sp!, {r0-r12, lr}

	# call the C IRQ handler
	bl c_irq_handler

	# restore the regs
	ldmfd sp!, {r0-r12,lr}

	# deactivate
	b deactivate

