//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/bridge/intelpciisa/intelpciisa.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>

/*
* perform device instance specific init here
*/
void deviceInitpciisa(struct device* dev){
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
}

void pciisaSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitpciisa;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
    devicemgr_register_device(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void bridge_register_pciisa() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_ISA, &pciisaSearchCB);
}
