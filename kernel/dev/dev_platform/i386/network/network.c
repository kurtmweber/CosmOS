// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev_platform/i386/network/network.h>

void network_devicemgr_register_devices() {
 //   rtl8139_devicemgr_register_devices();
    ne2000pci_devicemgr_register_devices();
 //   ne2000isa_devicemgr_register_devices();
  //  e1000_devicemgr_register_devices();
 //   vnic_devicemgr_register_devices();
}
