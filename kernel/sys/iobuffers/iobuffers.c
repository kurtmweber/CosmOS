//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/collection/bitmap/bitmap.h>
#include <sys/debug/assert.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/panic/panic.h>

#define SIXTY_FOUR_MEGA_BYTES 0x4000000

void* iobuffers_buffer_end_adddress(uint32_t start, uint32_t count);

uint64_t io_buf = 0;

struct arraylist* buffer_list = 0;
struct bitmap* map = 0;

struct iobuffers_buffer {
    uint64_t start_page;
    uint64_t num_pages;
    void* address;
};

void iobuffers_init() {
    if (0 == io_buf) {
        kprintf("   io_buf is null, no IO Buffer consfigured\n");
        return;
    }
    ASSERT_NOT_NULL(io_buf);
    uint64_t end_of_buffers = (uint64_t)iobuffers_buffer_end_adddress(0, IOBUFFERS_NUMBER);

    kprintf("   IO Buffers has %llu pages of size %#llX at virtual address %#llX-%#llX\n", IOBUFFERS_NUMBER, IOBUFFERS_BUFFER_SIZE, io_buf, end_of_buffers);
    ASSERT(end_of_buffers < (uint64_t)SIXTY_FOUR_MEGA_BYTES);

    buffer_list = arraylist_new(IOBUFFERS_NUMBER);
    map = bitmap_new(IOBUFFERS_NUMBER);
}

/*
 * check for a contiguous chunk of pages, at a start location
 */
/*
 * count is the number of pages we need
 */
uint8_t iobuffers_is_free_pages(uint32_t start, uint32_t count) {
    ASSERT_NOT_NULL(count);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);

    for (uint32_t i = 0; i < count; i++) {
        // indexed off the end
        if ((start + i) > IOBUFFERS_NUMBER) {
            return 0;
        }
        // check if used
        if (bitmap_get(map, start + i) > 0) {
            return 0;
        }
        return 1;
    }
}

uint32_t iobuffers_calc_num_pages(uint32_t size) {
    ASSERT_NOT_NULL(size);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);

    if (size <= IOBUFFERS_BUFFER_SIZE) {
        return 1;
    } else {
        uint32_t ret = size / IOBUFFERS_BUFFER_SIZE;
        if ((size % IOBUFFERS_BUFFER_SIZE) > 0) {
            ret = ret + 1;
        }
        return ret;
    }
}

void* iobuffers_address(uint32_t start) {
    return (void*)(((uint64_t)io_buf) + (start * IOBUFFERS_BUFFER_SIZE));
}
void* iobuffers_mark_pages_free(uint32_t start, uint32_t count) {
    ASSERT_NOT_NULL(count);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);
    // mark the pages
    for (uint32_t i = 0; i < count; i++) {
        bitmap_set(map, start + i, 1);
    }
}

void iobuffers_mark_pages_used(uint32_t start, uint32_t count) {
    ASSERT_NOT_NULL(count);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);
    // mark the pages
    for (uint32_t i = 0; i < count; i++) {
        bitmap_set(map, start + i, 1);
    }
}

void* iobuffers_buffer_end_adddress(uint32_t start, uint32_t count) {
    return iobuffers_address(start + count) + (IOBUFFERS_BUFFER_SIZE - 1);
}

void* iobuffers_request_buffer(uint32_t size) {
    ASSERT_NOT_NULL(size);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);

    uint32_t page_count = iobuffers_calc_num_pages(size);
    //   kprintf("   iobuffers_request_buffer for size %llu requires %llu pages of size %#hX \n", size, page_count, IOBUFFERS_BUFFER_SIZE);

    for (uint32_t i = 0; i < (IOBUFFERS_NUMBER - page_count); i++) {
        if (iobuffers_is_free_pages(i, page_count)) {
            // mark used
            iobuffers_mark_pages_used(i, page_count);
            // add a buffer record
            struct iobuffers_buffer* b = (struct iobuffers_buffer*)kmalloc(sizeof(struct iobuffers_buffer));
            b->start_page = i;
            b->num_pages = page_count;
            b->address = iobuffers_address(i);
            arraylist_add(buffer_list, b);

            //    kprintf("   iobuffers_request_buffer found free pages at index %llu, address %#hX-%#hX for base address %#hX\n", i, b->address, iobuffers_buffer_end_adddress(b->start_page, b->num_pages), io_buf);
            return b->address;
        }
    }
}

void iobuffers_release_buffer(void* buffer) {
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(map);
    ASSERT_NOT_NULL(buffer_list);
    /*
     * find the buffer
     */
    for (uint32_t i = 0; i < arraylist_count(buffer_list); i++) {
        struct iobuffers_buffer* buffer_record = (struct iobuffers_buffer*)arraylist_get(buffer_list, i);
        ASSERT_NOT_NULL(buffer_record);
        if (buffer_record->address == buffer) {
            // mark free
            iobuffers_mark_pages_free(buffer_record->start_page, buffer_record->num_pages);
            // remove record
            arraylist_remove(buffer_list, i);
            kfree(buffer_record);
            return;
        }
    }
    panic("unable to find record of io buffer");
}

uint32_t iobuffers_total_pages() {
    return IOBUFFERS_NUMBER;
}

uint32_t iobuffers_used_pages() {
    uint32_t ret = 0;
    for (uint32_t i = 0; i < arraylist_count(buffer_list); i++) {
        struct iobuffers_buffer* buffer_record = (struct iobuffers_buffer*)arraylist_get(buffer_list, i);
        ASSERT_NOT_NULL(buffer_record);
        ret += buffer_record->num_pages;
    }
    return ret;
}