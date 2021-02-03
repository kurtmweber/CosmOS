//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/partition_table_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_part_table.h>

/*
 * read 1 sector.  "data" is at least as big as sector_size
 */
void partition_table_write(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                           uint8_t* data) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    ASSERT_NOT_NULL(data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    (*pt_api->write)(partition_table_device, partition_index, sector, data, 1);
}
/*
 * write 1 sector. "data" is at least as big as sector_size
 */
void partition_table_read(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                          uint8_t* data) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    ASSERT_NOT_NULL(data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    (*pt_api->read)(partition_table_device, partition_index, sector, data, 1);
}

/*
 * write "count" sectors starting at lba "sector".  "data" is at least as big as sector_size
 */
void partition_table_write_sectors(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                                   uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    ASSERT_NOT_NULL(data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    (*pt_api->write)(partition_table_device, partition_index, sector, data, count);
}
/*
 * read "count" sectors starting at lba "sector".  "data" is at least as big as sector_size
 */
void partition_table_read_sectors(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                                  uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    ASSERT_NOT_NULL(data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    (*pt_api->read)(partition_table_device, partition_index, sector, data, count);
}

uint16_t partition_table_sector_size(struct device* partition_table_device, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    panic("not implemented");
    return 0;
}

uint32_t partition_table_total_size(struct device* partition_table_device, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->deviceData);
    panic("not implemented");
    return 0;
}
