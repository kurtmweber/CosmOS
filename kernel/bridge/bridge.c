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

/*
* perform device instance specific init here
*/
void deviceInitBridge(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, pci_dev->irq);
}

void BridgeSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &deviceInitBridge;
    deviceinstance->deviceData = dev;
    deviceSetDescription(deviceinstance, "Bridge");
    registerDevice(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void bridge_register_devices() {
    pci_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_HOST, &BridgeSearchCB);
    pci_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_ISA, &BridgeSearchCB);
    pci_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_OTHER, &BridgeSearchCB);
}
