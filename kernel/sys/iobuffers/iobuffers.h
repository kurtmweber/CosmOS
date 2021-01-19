//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IOBUFFERS_H
#define _IOBUFFERS_H

#define IOBUFFERS_ALIGNMENT 0x10000   // 64k
#define IOBUFFERS_BUFFER_SIZE 0x1000  // 4k
#define IOBUFFERS_NUMBER 0x40         // 64

#define IOBUFFERS_TOTAL_SIZE IOBUFFERS_BUFFER_SIZE* IOBUFFERS_NUMBER

#include <types.h>

void iobuffers_init();
void* iobuffers_request_buffer(uint32_t size);
void iobuffers_release_buffer(void* buffer);
uint32_t iobuffers_total_pages();
uint32_t iobuffers_used_pages();

// this set up by mm
extern void* io_buf;

#endif