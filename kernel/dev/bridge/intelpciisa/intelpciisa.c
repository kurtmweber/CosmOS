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
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id);
}

void pciisaSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = new_device();
    deviceinstance->init =  &deviceInitpciisa;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = BRIDGE;
    device_set_description(deviceinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
    register_device(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void bridge_register_pciisa() {
    pci_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_ISA, &pciisaSearchCB);
}
