//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/ethernet/ethernet_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>

void ethernet_read(struct device* dev, uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->api);
    struct deviceapi_ethernet* rtc_api = (struct deviceapi_ethernet*) dev->api;
}

void ethernet_write(struct device* dev, uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->api);
    struct deviceapi_ethernet* rtc_api = (struct deviceapi_ethernet*) dev->api;
}
