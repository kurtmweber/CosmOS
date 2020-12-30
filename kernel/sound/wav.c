//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sound/wav.h>
#include <debug/assert.h>
#include <console/console.h>

uint8_t* wav_pcm_start(struct wav_header* header) {
    ASSERT_NOT_NULL(header, "header must not be null");
	uint8_t* ret =(uint8_t*) ((uint64_t) header)+sizeof(struct wav_header);
	kprintf("PCM start %#X\n", (uint64_t) ret);
    return ret;
}

