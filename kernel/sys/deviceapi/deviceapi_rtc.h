//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all RTC devices will implement
 */
#ifndef _DEVICEAPI_RTC_H
#define _DEVICEAPI_RTC_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef struct rtc_time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t weekday;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} rtc_time_t;

typedef rtc_time_t (*rtc_time_function)(struct device* dev);
typedef void (*rtc_event)();
typedef void (*rtc_subscribe_function)(rtc_event event);

struct deviceapi_rtc {
    rtc_time_function rtc_time;
    rtc_subscribe_function subscribe;
};

#endif