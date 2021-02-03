//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEV_H
#define _DEV_H

#include <dev/fs/swap/swap.h>
#include <dev/ramdisk/ramdisk.h>
#include <dev/virtio/virtio.h>

#ifdef TARGET_PLATFORM_i386

#include <dev/x86-64/ata/ata_controller.h>
#include <dev/x86-64/bda/bda.h>
#include <dev/x86-64/bridge/bridge.h>
#include <dev/x86-64/cmos/cmos.h>
#include <dev/x86-64/cpu/cpu.h>
#include <dev/x86-64/ebda/ebda.h>
#include <dev/x86-64/floppy/floppy.h>
#include <dev/x86-64/isadma/isadma.h>
#include <dev/x86-64/keyboard/keyboard.h>
#include <dev/x86-64/mouse/mouse.h>
#include <dev/x86-64/network/network.h>
#include <dev/x86-64/parallel/parallel.h>
#include <dev/x86-64/pci/pci.h>
#include <dev/x86-64/pci_ehci/pci_ehci.h>
#include <dev/x86-64/pic/pic.h>
#include <dev/x86-64/pit/pit.h>
#include <dev/x86-64/rtc/rtc.h>
#include <dev/x86-64/sdhci/sdhci.h>
#include <dev/x86-64/serial/serial.h>
#include <dev/x86-64/sound/sound.h>
#include <dev/x86-64/speaker/speaker.h>
#include <dev/x86-64/usb_ehci/usb_ehci.h>
#include <dev/x86-64/vga/vga.h>

#else

#include <dev/arm/pl101/pl101.h>

#endif

#endif