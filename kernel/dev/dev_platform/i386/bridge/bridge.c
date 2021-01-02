//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev_platform/i386/bridge/bridge.h>
#include <system/system_platform/i386/interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <system/devicemgr/devicemgr.h>
#include <system/console/console.h>
#include <dev/dev_platform/i386/bridge/inteli440fx/inteli440fx.h>
#include <dev/dev_platform/i386/bridge/intel82371ab/intel82371ab.h>
#include <dev/dev_platform/i386/bridge/intelpciisa/intelpciisa.h>

/**
* find all bridge devices and register them
*/
void bridge_devicemgr_register_devices() {
    i440fx_bridge_register();
    i982371_bridge_register();
    pciisa_bridge_register();
}
