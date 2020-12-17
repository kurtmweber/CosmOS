/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <rtc/rtc.h>
#include <interrupts/interrupt_router.h>
#include <devicemgr/devicemgr.h>
#include <timing/sleep.h>

#define RTC_IRQ_NUMBER 8
#define RTC_FREQ 1024

#define CMOS_REGISTER_SELECT_PORT	0x70
#define CMOS_REGISTER_DATA_PORT		0x71

uint16_t rtc_freq;

typedef enum rtc_registers{
	RTC_REGISTER_SECOND = 0x00,
	RTC_REGISTER_MINUTE = 0x02,
	RTC_REGISTER_HOUR = 0x04,
	RTC_REGISTER_WEEKDAY = 0x06,
	RTC_REGISTER_MONTHDAY = 0x07,
	RTC_REGISTER_MONTH = 0x08,
	RTC_REGISTER_YEAR = 0x09,
	RTC_REGISTER_STATUS_A = 0x0A,
	RTC_REGISTER_STATUS_B = 0x0B,
	RTC_REGISTER_STATUS_C = 0x0C,
	RTC_REGISTER_CENTURY = 0x32
} rtc_registers;

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

uint8_t rtc_read_register(uint8_t reg){
	uint8_t b;
	
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, reg);
	b = asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_sti();
	
	return b;
}

void rtc_handle_irq(stackFrame *frame){
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
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, RTC_IRQ_NUMBER);
	rtc_freq = RTC_FREQ;
	sleep_countdown = 0;
	
	asm_cli();
	rtc_write_register(RTC_REGISTER_STATUS_B, 0x40);	// bit 6 turns on interrupt
	registerInterruptHandler(RTC_IRQ_NUMBER, &rtc_handle_irq);
}

/*
* find all RTC devices and register them
*/
void rtc_register_devices(){
	/*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceSetDescription(deviceinstance, "RTC");
	deviceinstance->devicetype = RTC;
	deviceinstance->init =  &deviceInitRTC;
	registerDevice(deviceinstance);
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


