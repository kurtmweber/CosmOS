/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <mm/mm.h>
#include <pci/pci.h>
#include <pci/devicetree.h>

void pci_init(){	
	kprintf("Initializing PCI bus...\n");
	
	pci_devices = listNew();

	pci_scan();
	
	return;
}
