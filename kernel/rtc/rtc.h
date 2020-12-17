//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RTC_H
#define _RTC_H

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

extern uint16_t rtc_freq;

void rtc_handle_irq();
void rtc_register_devices();
rtc_time_t rtc_time();

uint8_t rtc_read_register(uint8_t reg);
void rtc_write_register(uint8_t reg, uint8_t val);

#endif

