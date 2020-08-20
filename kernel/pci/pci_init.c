/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_INIT_C
#define _PCI_INIT_C

#include <types.h>
#include <console/console.h>
#include <pci/pci.h>

void pci_init(){
	kprintf("Initializing PCI bus...\n");
	
	pci_scan();
	
	return;
}

#endif