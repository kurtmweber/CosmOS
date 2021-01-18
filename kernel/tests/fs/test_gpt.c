//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/pt/guid_pt.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/fs/test_gpt.h>

void test_gpt() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct device* dev = guid_pt_attach(dsk);
        guid_pt_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
