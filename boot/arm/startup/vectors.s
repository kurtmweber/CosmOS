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

.global __vectors_start__
.global __vectors_end__

#
# ARM vector table
#
__vectors_start__:
	ldr			PC, reset_handler_addr
	ldr			PC, undef_handler_addr
	ldr			PC, swi_handler_addr
	ldr			PC, prefetch_abort_handler_addr
	ldr			PC, data_abort_handler_addr
	b			.
	ldr			PC, irq_handler_addr
	ldr			PC, fiq_handler_addr
 
reset_handler_addr: 			.word reset_handler
undef_handler_addr: 			.word undef_handler
swi_handler_addr:				.word swi_handler
prefetch_abort_handler_addr:	.word prefetch_abort_handler
data_abort_handler_addr:		.word data_abort_handler
irq_handler_addr:				.word irq_handler
fiq_handler_addr:				.word fiq_handler
 
__vectors_end__:

# copyright message
.string "Copyright (c) Tom Everett. All Rights Reserved."

.end
