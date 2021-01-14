//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEV_H
#define _DEV_H

#ifdef TARGET_PLATFORM_i386

#include <dev/i386/ac97/ac97.h>
#include <dev/i386/adlib/adlib.h>
#include <dev/i386/ata/ata_controller.h>
#include <dev/i386/bda/bda.h>
#include <dev/i386/bridge/bridge.h>
#include <dev/i386/cmos/cmos.h>
#include <dev/i386/cpu/cpu.h>
#include <dev/i386/display/display.h>
#include <dev/i386/ebda/ebda.h>
#include <dev/i386/floppy/floppy.h>
#include <dev/i386/isadma/isadma.h>
#include <dev/i386/keyboard/keyboard.h>
#include <dev/i386/mouse/mouse.h>
#include <dev/i386/network/network.h>
#include <dev/i386/parallel/parallel.h>
#include <dev/i386/pci/pci.h>
#include <dev/i386/pci_ehci/pci_ehci.h>
#include <dev/i386/pic/pic.h>
#include <dev/i386/pit/pit.h>
#include <dev/i386/rtc/rtc.h>
#include <dev/i386/sb16/sb16.h>
#include <dev/i386/serial/serial.h>
#include <dev/i386/speaker/speaker.h>
#include <dev/i386/usb_ehci/usb_ehci.h>
#include <dev/ramdisk/ramdisk.h>
#include <dev/virtio/virtio.h>
#else

#include <dev/arm/pl101/pl101.h>

#endif

#endif