#
# Copyright (C) khubla.com - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited#
# Proprietary and confidential
# Written by Tom Everett <tom@khubla.com>, 2014-2017
#

# processor modes
.set    USER_MODE, 			0x10
.set    FIQ_MODE, 			0x11
.set    IRQ_MODE, 			0x12
.set    SUPERVISOR_MODE, 	0x13
.set    ABORT_MODE,			0x17
.set    UNDEFINED_MODE, 	0x1B
.set    SYSTEM_MODE, 		0x1F

# IRQ and FIQ disable
.set	IRQ_DISABLE,		0x80
.set	FIQ_DISABLE,		0x40

# MODE_SYSTEM | IRQ_DISABLE | FIQ_DISABLE
.set	SYSTEM_MODE_NO_INTERRUPTS,		0xDF

# MODE_SUPERVISOR | IRQ_DISABLE | FIQ_DISABLE
.set	SUPERVISOR_MODE_NO_INTERRUPTS,	0xD3

# MODE_IRQ | IRQ_DISABLE | FIQ_DISABLE
.set	IRQ_MODE_NO_INTERRUPTS,	0xD2

# MODE_IRQ | FIQ_DISABLE
.set	IRQ_MODE_NO_IRQ,	0x92

# MODE_USER | IRQ_DISABLE | FIQ_DISABLE
.set	USER_MODE_NO_INTERRUPTS,	0xD0
