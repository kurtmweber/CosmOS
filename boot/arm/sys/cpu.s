#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

.text
.code 32
.align 4

// the "C" prototype for these are in asmcalls.hpp

.global cpuid
cpuid:
	mrc		p15, #0, r0, c0, c0, #0
	mov		pc, lr


