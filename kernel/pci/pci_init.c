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
#include <mm/mm.h>
#include <pci/pci.h>
#include <pci/devicetree.h>

void pci_init(){
	uint16_t i;
	
	kprintf("Initializing PCI bus...\n");
	
	pci_devices = 0;
	
	pci_scan();
	
	return;
}

#endif