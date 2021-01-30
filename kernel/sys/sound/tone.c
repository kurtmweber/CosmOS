
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/rawdata/rawdata.h>
#include <sys/sound/tone.h>
#include <sys/sound/wav.h>

struct wav_header* sound_get_tone() {
    uint64_t start = (uint64_t)&_tone_s;
    uint64_t end = (uint64_t)&_tone_e;
    uint64_t size = (uint64_t)&_tone_e - (uint64_t)&_tone_s;

    // show the tone data.  byte size should be the same as tone.wav
    kprintf("Data for tone.wav is from %#llX to %#llX with byte size %llu\n", start, end, size);

    struct wav_header* ret = (struct wav_header*)(uint64_t)&_tone_s;

    ASSERT(ret->riff_header[0] == 'R');
    ASSERT(ret->riff_header[1] == 'I');
    ASSERT(ret->riff_header[2] == 'F');
    ASSERT(ret->riff_header[3] == 'F');

    return ret;
}
