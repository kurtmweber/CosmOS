//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <bridge/bridge.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <device/device.h>
#include <console/console.h>
#include <pci/pci.h>


void bridge_init() {
    uint16_t i = 0;
    for (i = 0; i < num_pci_devices; i++){
        if (pci_devices[i].pci_class == PCI_CLASS_BRIDGE)  {
            if ((pci_bridge_subclass_codes)pci_devices[i].pci_subclass == PCI_BRIDGE_SUBCLASS_HOST){
    		    kprintf("Host Bridge found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			    kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
            }
            else if ((pci_bridge_subclass_codes)pci_devices[i].pci_subclass == PCI_BRIDGE_SUBCLASS_ISA){
    		    kprintf("ISA Bridge found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			    kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
            }
            else if ((pci_bridge_subclass_codes)pci_devices[i].pci_subclass == PCI_BRIDGE_SUBCLASS_OTHER){
    		    kprintf("Other Bridge found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			    kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
            }
        }
    }
}
