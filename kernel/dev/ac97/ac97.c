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
#include <devicemgr/deviceapi/deviceapi_dsp.h>
#include <panic/panic.h>

// https://wiki.osdev.org/AC97

#define AC97_RESET                 0x00
#define AC97_MASTER_VOLUME         0x02
#define AC97_AUX_OUT_VOLUME        0x04
#define AC97_MONO_VOLUME           0x06
#define AC97_MASTER_TONE           0x08

#define AC97_PC_BEEP_VOLUME        0x0A
#define AC97_PHONE_VOLUME          0x0C
#define AC97_MIC_VOLUME            0x0E
#define AC97_LINEIN_VOLUME         0x10
#define AC97_CD_VOLUME             0x12
#define AC97_VIDEO_VOLUME          0x14
#define AC97_AUXIN_VOLUME          0x16
#define AC97_PCMOUT_VOLUME         0x18
#define AC97_RECORD_SELECT         0x1A
#define AC97_RECORD_GAIN           0x1C
#define AC97_RECORD_GAIN_MIC       0x1E
#define AC97_GENERAL_PURPOSE       0x20
#define AC97_3D_CONTROL            0x22

void ac97_handle_irq(stackFrame *frame) {
}

/*
* perform device instance specific init here
*/
void deviceInitAC97(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
   	kprintf("Init %s at IRQ %llu\n",dev->description, dev->pci->irq);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &ac97_handle_irq);
}

void AC97PCISearchCB(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitAC97;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = DSP;
    devicemgr_set_device_description(deviceinstance, "Intel 82801AA AC97");
	/*
	* device api
	*/
	struct deviceapi_dsp* api = (struct deviceapi_dsp*) kmalloc (sizeof(struct deviceapi_dsp));
	deviceinstance->api = api;
	/*
	* register
	*/
	devicemgr_register_device(deviceinstance);
}

void ac97_devicemgr_register_devices(){
	pci_devicemgr_search_device(PCI_CLASS_MULTIMEDIA,0x01, 0x8086,0x2415, &AC97PCISearchCB);
}

