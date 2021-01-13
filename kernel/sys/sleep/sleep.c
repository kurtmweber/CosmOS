/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>

volatile uint64_t sleep_countdown;

#define RTC_FREQ 1024
uint16_t rtc_freq = RTC_FREQ;

void sleep_update() {
    if (sleep_countdown) {
        sleep_countdown--;
        // kprintf("%llu\n", sleep_countdown);
    }

    return;
}

void sleep_wait(uint64_t milliseconds) {
    uint64_t tc;  // temporary count while we're working up to full value

    // to avoid getting progressively farther off due to the effects of integer division, we start by finding out
    // how many full seconds we're waiting for, and multiply that by the frequency
    tc = rtc_freq * (milliseconds / 1000);

    // and then we add the fractional seconds
    // TODO: handle rounding error better
    tc += ((milliseconds % 1000) * (rtc_freq / 1000));

    sleep_countdown = tc;

    // rtc_subscribe(&sleep_update);

    while (sleep_countdown) {
        asm_hlt();
    }

    return;
}
