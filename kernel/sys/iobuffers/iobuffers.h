//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IOBUFFERS_H
#define _IOBUFFERS_H

#include <sys/i386/mm/mm.h>
#include <types.h>

#define IOBUFFERS_ALIGNMENT 0x10000   // 64k
#define IOBUFFERS_BUFFER_SIZE 0x1000  // 4k

// IO_SPACE_SIZE is defined in mm.h
#define IOBUFFERS_NUMBER (IO_SPACE_SIZE / IOBUFFERS_BUFFER_SIZE)

void iobuffers_init();
void* iobuffers_request_buffer(uint32_t size);
void iobuffers_release_buffer(void* buffer);
uint32_t iobuffers_total_pages();
uint32_t iobuffers_used_pages();

// this set up by mm
extern uint64_t io_buf;

#endif