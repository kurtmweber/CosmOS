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

.global disableinterrupts
disableinterrupts:
	.func
	cpsid i
	mov pc, lr
	.endfunc

.global enableinterrupts
enableinterrupts:
	.func
	cpsie i
	mov pc, lr
	.endfunc
