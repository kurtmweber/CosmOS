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

// the "C" prototype for this is in asmcalls.hpp

.global wfi
wfi:
	mov     r0, #0
	mcr     p15, 0, r0, c7, c0, 4
	bx lr
