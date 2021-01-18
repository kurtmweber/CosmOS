//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __TESTS_H
#define __TESTS_H

#include <tests/dev/test_ata.h>
#include <tests/dev/test_bda.h>
#include <tests/dev/test_cpu.h>
#include <tests/dev/test_floppy.h>
#include <tests/dev/test_parallel.h>
#include <tests/dev/test_pit.h>
#include <tests/dev/test_ramdisk.h>
#include <tests/dev/test_rtc.h>
#include <tests/dev/test_rtl8139.h>
#include <tests/dev/test_sb16.h>
#include <tests/dev/test_serial.h>
#include <tests/dev/test_serial_console.h>
#include <tests/dev/test_speaker.h>
#include <tests/dev/test_swap.h>
#include <tests/dev/test_vblock.h>
#include <tests/dev/test_vga_console.h>
#include <tests/fs/test_cfs.h>
#include <tests/fs/test_fat.h>
#include <tests/fs/test_gpt.h>
#include <tests/fs/test_sfs.h>
#include <tests/fs/test_tfs.h>
#include <tests/sys/test_array.h>
#include <tests/sys/test_arraylist.h>
#include <tests/sys/test_debug.h>
#include <tests/sys/test_linkedlist.h>
#include <tests/sys/test_malloc.h>
#include <tests/sys/test_mm.h>
#include <tests/sys/test_ringbuffer.h>
#include <tests/sys/test_string.h>
#include <tests/sys/test_tree.h>
#include <types.h>

/*
 * some tests we always run on startup, just to be sure
 */
void tests_run();

#endif