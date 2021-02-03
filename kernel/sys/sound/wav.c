//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sound/wav.h>

uint8_t* wav_pcm_start(struct wav_header* header) {
    ASSERT_NOT_NULL(header);
    uint8_t* ret = (uint8_t*)((uint64_t)header) + sizeof(struct wav_header);
    kprintf("PCM start %#llX\n", (uint64_t)ret);
    return ret;
}
