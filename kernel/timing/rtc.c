/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _RTC_C
#define _RTC_C

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <timing/timing.h>
#include <interrupts/interrupt_router.h>
#include <device/device.h>

#define RTC_IRQ_NUMBER 8
#define RTC_FREQ 1024

void rtc_handle_irq(){
#ifdef RTC_SLEEP
	sleep_update();
#endif
	
	// have to read status register C in order for irq to fire again
	rtc_read_register(RTC_REGISTER_STATUS_C);
	return;
}

/*
* perform device instance specific init here
*/
void deviceInitRTC(struct device* dev){
    kprintf("Init RTC\n");
	rtc_freq = RTC_FREQ;
	sleep_countdown = 0;
	
	asm_cli();
	rtc_write_register(RTC_REGISTER_STATUS_B, 0x40);	// bit 6 turns on interrupt
	registerInterruptHandler(RTC_IRQ_NUMBER, &rtc_handle_irq);
}

void rtc_register_devices(){
	/*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceinstance->init =  &deviceInitRTC;
	registerDevice(deviceinstance);
}

uint8_t rtc_read_register(uint8_t reg){
	uint8_t b;
	
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, reg);
	b = asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_sti();
	
	return b;
}

rtc_time_t rtc_time(){
	rtc_time_t a, b;
	
	a.second = rtc_read_register(RTC_REGISTER_SECOND);
	a.minute = rtc_read_register(RTC_REGISTER_MINUTE);
	a.hour = rtc_read_register(RTC_REGISTER_HOUR);
	a.weekday = rtc_read_register(RTC_REGISTER_WEEKDAY);
	a.monthday = rtc_read_register(RTC_REGISTER_MONTHDAY);
	a.month = rtc_read_register(RTC_REGISTER_MONTH);
	a.year = rtc_read_register(RTC_REGISTER_YEAR);
	a.century = rtc_read_register(RTC_REGISTER_CENTURY);
	
	b.second = rtc_read_register(RTC_REGISTER_SECOND);
	b.minute = rtc_read_register(RTC_REGISTER_MINUTE);
	b.hour = rtc_read_register(RTC_REGISTER_HOUR);
	b.weekday = rtc_read_register(RTC_REGISTER_WEEKDAY);
	b.monthday = rtc_read_register(RTC_REGISTER_MONTHDAY);
	b.month = rtc_read_register(RTC_REGISTER_MONTH);
	b.year = rtc_read_register(RTC_REGISTER_YEAR);
	b.century = rtc_read_register(RTC_REGISTER_CENTURY);
	
	if ((a.second == b.second) && (a.minute == b.minute) &&
		(a.hour == b.hour) && (a.weekday == b.weekday) &&
		(a.monthday == b.monthday) && (a.month == b.month) &&
		(a.year == b.year) && (a.century == b.century)){
		return b;
	}
	
	do {
		a = b;
		
		b.second = rtc_read_register(RTC_REGISTER_SECOND);
		b.minute = rtc_read_register(RTC_REGISTER_MINUTE);
		b.hour = rtc_read_register(RTC_REGISTER_HOUR);
		b.weekday = rtc_read_register(RTC_REGISTER_WEEKDAY);
		b.monthday = rtc_read_register(RTC_REGISTER_MONTHDAY);
		b.month = rtc_read_register(RTC_REGISTER_MONTH);
		b.year = rtc_read_register(RTC_REGISTER_YEAR);
		b.century = rtc_read_register(RTC_REGISTER_CENTURY);
	} while ((a.second != b.second) || (a.minute != b.minute) ||
		(a.hour != b.hour) || (a.weekday != b.weekday) ||
		(a.monthday != b.monthday) || (a.month != b.month) ||
		(a.year != b.year) || (a.century != b.century));
	
	return b;	
}

void rtc_write_register(uint8_t reg, uint8_t val){
	uint8_t pv;
	
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);	// disable NMI so we don't screw up the CMOS RTC irreparably
	pv = asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);
	asm_out_b(CMOS_REGISTER_DATA_PORT, pv | val);
	
	asm_out_b(CMOS_REGISTER_SELECT_PORT, asm_in_b(CMOS_REGISTER_SELECT_PORT) & 0x7F);	// re-enable nmi
	asm_sti();
	
	return;
}

#endif