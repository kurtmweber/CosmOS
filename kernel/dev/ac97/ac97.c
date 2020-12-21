/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/ac97/ac97.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <interrupts/interrupt_router.h>
#include <dev/pci/pci.h>

void ac97_handle_irq(stackFrame *frame) {
}

/*
* perform device instance specific init here
*/
void deviceInitAC97(struct device* dev){
	struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
   	kprintf("Init %s at IRQ %llu\n",dev->description, pci_dev->irq);
    interrupt_router_register_interrupt_handler(pci_dev->irq, &ac97_handle_irq);
}

void AC97PCISearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitAC97;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = DSP;
    devicemgr_set_device_description(deviceinstance, "Intel 82801AA AC97");
    devicemgr_register_device(deviceinstance);
}

void ac97_devicemgr_register_devices(){
	pci_devicemgr_search_device(PCI_CLASS_MULTIMEDIA,0x01, 0x8086,0x2415, &AC97PCISearchCB);
}

