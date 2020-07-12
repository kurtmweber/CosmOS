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
#include <timing/timing.h>

uint8_t rtc_read_register(uint8_t reg){
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, reg);
	return asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_sti();
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

#endif