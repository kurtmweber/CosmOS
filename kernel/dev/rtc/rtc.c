/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <dev/rtc/rtc.h>
#include <dev/rtc/cmos.h>
#include <interrupts/interrupt_router.h>
#include <devicemgr/devicemgr.h>
#include <sleep/sleep.h>

#define RTC_IRQ_NUMBER 8
#define RTC_PORT 0x40

typedef enum rtc_registers{
	RTC_REGISTER_SECOND = 	0x00,
	RTC_REGISTER_MINUTE = 	0x02,
	RTC_REGISTER_HOUR = 	0x04,
	RTC_REGISTER_WEEKDAY = 	0x06,
	RTC_REGISTER_MONTHDAY = 0x07,
	RTC_REGISTER_MONTH = 	0x08,
	RTC_REGISTER_YEAR = 	0x09,
	RTC_REGISTER_STATUS_A = 0x0A,
	RTC_REGISTER_STATUS_B = 0x0B,
	RTC_REGISTER_STATUS_C = 0x0C,
	RTC_REGISTER_CENTURY = 	0x32
} rtc_registers;

void rtc_handle_irq(stackFrame *frame){
#ifdef RTC_SLEEP
	sleep_update();
#endif
	
	// have to read status register C in order for irq to fire again
	cmos_read_register(RTC_REGISTER_STATUS_C);
	return;
}

/*
* perform device instance specific init here
*/
void deviceInitRTC(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, RTC_IRQ_NUMBER);
	
	asm_cli();
	cmos_write_register(RTC_REGISTER_STATUS_B, RTC_PORT);	// bit 6 turns on interrupt
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
	
	a.second = cmos_read_register(RTC_REGISTER_SECOND);
	a.minute = cmos_read_register(RTC_REGISTER_MINUTE);
	a.hour = cmos_read_register(RTC_REGISTER_HOUR);
	a.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
	a.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
	a.month = cmos_read_register(RTC_REGISTER_MONTH);
	a.year = cmos_read_register(RTC_REGISTER_YEAR);
	a.century = cmos_read_register(RTC_REGISTER_CENTURY);
	
	b.second = cmos_read_register(RTC_REGISTER_SECOND);
	b.minute = cmos_read_register(RTC_REGISTER_MINUTE);
	b.hour = cmos_read_register(RTC_REGISTER_HOUR);
	b.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
	b.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
	b.month = cmos_read_register(RTC_REGISTER_MONTH);
	b.year = cmos_read_register(RTC_REGISTER_YEAR);
	b.century = cmos_read_register(RTC_REGISTER_CENTURY);
	
	if ((a.second == b.second) && (a.minute == b.minute) &&
		(a.hour == b.hour) && (a.weekday == b.weekday) &&
		(a.monthday == b.monthday) && (a.month == b.month) &&
		(a.year == b.year) && (a.century == b.century)){
		return b;
	}
	
	do {
		a = b;
		
		b.second = cmos_read_register(RTC_REGISTER_SECOND);
		b.minute = cmos_read_register(RTC_REGISTER_MINUTE);
		b.hour = cmos_read_register(RTC_REGISTER_HOUR);
		b.weekday = cmos_read_register(RTC_REGISTER_WEEKDAY);
		b.monthday = cmos_read_register(RTC_REGISTER_MONTHDAY);
		b.month = cmos_read_register(RTC_REGISTER_MONTH);
		b.year = cmos_read_register(RTC_REGISTER_YEAR);
		b.century = cmos_read_register(RTC_REGISTER_CENTURY);
	} while ((a.second != b.second) || (a.minute != b.minute) ||
		(a.hour != b.hour) || (a.weekday != b.weekday) ||
		(a.monthday != b.monthday) || (a.month != b.month) ||
		(a.year != b.year) || (a.century != b.century));
	
	return b;	
}


