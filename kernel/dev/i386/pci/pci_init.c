/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/pci/devicetree.h>
#include <dev/i386/pci/pci.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <types.h>

void pci_init() {
    kprintf("Initializing PCI bus...\n");

    pci_devices = list_new();

    pci_scan();

    return;
}
