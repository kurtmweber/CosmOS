//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_iobuffers.h>

void test_iobuffers() {
    kprintf("Testing iobuffers\n");

    uint16_t used = iobuffers_used_pages();
    ASSERT_NOT_NULL(used);
    uint16_t total = iobuffers_total_pages();
    ASSERT_NOT_NULL(total);

    // allocate 8k
    void* m = iobuffers_request_buffer(1024 * 8);
    ASSERT(iobuffers_used_pages() > used);
    iobuffers_release_buffer(m);
    ASSERT(iobuffers_used_pages() == used);
}
