/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/console/drivers/drivers.h>
#include <sys/deviceapi/deviceapi_cpu.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/deviceapi/deviceapi_rtc.h>
#include <sys/deviceapi/deviceapi_serial.h>
#include <sys/deviceapi/deviceapi_speaker.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs.h>
#include <sys/i386/interrupts/idt.h>
#include <sys/i386/mm/mm.h>
#include <sys/i386/process/gdt.h>
#include <sys/i386/video/vga/vga.h>
#include <sys/i386/video/video.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/sync/sync.h>
#include <tests/tests.h>
#include <types.h>

void dev_tests();

void CosmOS() {
    /*
     * switch to new GDT.  this is in place bug seems to cause crashes....
     */
    //  gdt_install();
    /*
     * start video
     */
    video_init();
    video_select_driver(VIDEO_DRIVER_VGA);
    video_select_mode(VIDEO_MODE_TEXT);

    console_driver_interface_init();
    /*
     * Put a hello message on the video, just so we know it's ok....
     */
    console_select_driver(CONSOLE_DRIVER_VGA);
    kprintf("Welcome to CosmOS 0.1\n");
    /*
     * ok, output to the serial console now
     */
    console_select_driver(CONSOLE_DRIVER_SERIAL);

    /*
     * Hello!
     */
    kprintf("Loading CosmOS 0.1\n");

    kprintf("Initializing IDT...\n");
    idt_init();

    kprintf("Initializing Spinlocks...\n");
    spinlocks_init();

    kprintf("Initializing MMU...\n");
    mmu_init();

    kprintf("Initializing Interrupt Routing...\n");
    interrupt_router_init();

    /*
     * init the device registry
     */
    kprintf("Initializing Device Registry...\n");
    devicemgr_init();

    /*
     * Register all devices
     */
    devicemgr_register_devices();
    kprintf("Registered %llu devices\n", devicemgr_device_count());

    /*
     * Init all devices
     */
    devicemgr_init_devices();
    kprintf("There are %llu devices\n", devicemgr_device_count());
    kprintf("\n");
    kprintf("************************************\n");
    kprintf("** Device Initialization Complete **\n");
    kprintf("************************************\n");
    kprintf("\n");

    asm_sti();

    /*
     * register file systems
     */
    fs_init();

    kprintf("\n");
    kprintf("************************************\n");
    kprintf("** Kernel Initialization Complete **\n");
    kprintf("************************************\n");
    kprintf("\n");

    /*
     * standard suite of tests to make sure things are as expected
     */
    tests_run();

    kprintf("\n");
    kprintf("************************************\n");
    kprintf("**     Sanity Tests Complete      **\n");
    kprintf("************************************\n");
    kprintf("\n");

    /*
     * enable interrupts
     */
    asm_sti();

    kprintf("\n");
    kprintf("************************************\n");
    kprintf("**       Welcome to CosmOS!       **\n");
    kprintf("************************************\n");
    kprintf("\n");

    dev_tests();
    /*
     * run various functions to show that things work....
     */
    // testFunctions();

    while (1) {
        asm_hlt();
    }
}

void dev_tests() {
    //	playsb16();
    //	floppyread();
    //	test_ata();
    //	test_vblock();
    //	test_ata();
    //	test_ramdisk();
    //	test_cfs();
    //	test_sfs();
    //	test_dfs();
    //	test_fat();
    //	test_list();
    //  test_bda();
    //  test_parallel();
    //    test_rtl8139();
    // test_gpt();
}
