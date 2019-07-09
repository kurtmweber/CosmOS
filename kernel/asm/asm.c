/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2019 Kurt M. Weber                              *
 * Released under he terms of the Social Justice License         *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

void asm_hlt(){
	asm volatile(
		"hlt"
	    );
	
	return;
}