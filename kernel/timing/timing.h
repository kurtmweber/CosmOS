/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _TIMING_H
#define _TIMING_H

#include <types.h>

#define CMOS_REGISTER_SELECT_PORT	0x70
#define CMOS_REGISTER_DATA_PORT		0x71

typedef struct rtc_time_t{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t weekday;
	uint8_t monthday;
	uint8_t month;
	uint8_t year;
	uint8_t century;
} rtc_time_t;

typedef enum rtc_registers{
	RTC_REGISTER_SECOND = 0x00,
	RTC_REGISTER_MINUTE = 0x02,
	RTC_REGISTER_HOUR = 0x04,
	RTC_REGISTER_WEEKDAY = 0x06,
	RTC_REGISTER_MONTHDAY = 0x07,
	RTC_REGISTER_MONTH = 0x08,
	RTC_REGISTER_YEAR = 0x09,
	RTC_REGISTER_CENTURY = 0x32
} rtc_registers;

#ifndef _RTC_C
rtc_time_t rtc_time();
#else
uint8_t rtc_read_register(uint8_t reg);
#endif

#endif