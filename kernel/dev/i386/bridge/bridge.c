//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/bridge/bridge.h>
#include <dev/i386/bridge/intel82371ab/intel82371ab.h>
#include <dev/i386/bridge/inteli440fx/inteli440fx.h>
#include <dev/i386/bridge/intelpciisa/intelpciisa.h>
#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>

/**
 * find all bridge devices and register them
 */
void bridge_devicemgr_register_devices() {
    i440fx_bridge_register();
    i982371_bridge_register();
    pciisa_bridge_register();
}
