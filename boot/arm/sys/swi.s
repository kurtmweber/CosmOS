#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

.extern c_swi_handler

/*
	R14_svc = PC + 4
	SPSR_svc = CPSR
	CPSR[4:0] = 0b10011 //Supervisor Mode
	CPSR[5] = 0 // ARM state
	CPSR[6] unchanged
	CPSR[7] = 1 // Disable IRQ
	PC = 0x00000008
*/
.global swi_handler
swi_handler:
	# store params
	stmfd sp!, {r0-r12, lr}

	# get the swi number into r3
	# r0-r1 are the parameters to the SWI
	ldr	r3,[lr,#-4]
	bic	r3,r3, #0xff000000

	# save pc into r2
	mov r2, lr

	# call the C SWI handler
	bl c_swi_handler

	# restore
	# the lr value we pushed is restored into PC
	# the ^ means we copy the SPSR into CPSR
	ldmfd sp!, {r0-r12, pc}^

