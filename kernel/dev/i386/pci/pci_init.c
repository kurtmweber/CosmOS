/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <system/console/console.h>
#include <system/platform/i386/mm/mm.h>
#include <dev/i386/pci/pci.h>
#include <dev/i386/pci/devicetree.h>

void pci_init(){	
	kprintf("Initializing PCI bus...\n");
	
	pci_devices = list_new();

	pci_scan();
	
	return;
}
