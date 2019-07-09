/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2019 Kurt M. Weber                              *
 * Released under he terms of the Social Justice License         *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include "asm/asm.h"

void JustOS(){
	volatile char *vidMem = (volatile char *)0xB8000;
	
	*vidMem++ = 'J';
	*vidMem++ = 0x0F;
	*vidMem++ = 'u';
	*vidMem++ = 0x0F;
	*vidMem++ = 's';
	*vidMem++ = 0x0F;
	*vidMem++ = 't';
	*vidMem++ = 0x0F;
	*vidMem++ = 'O';
	*vidMem++ = 0x0F;
	*vidMem++ = 'S';
	*vidMem++ = 0x0F;
	
	asm_hlt();
}