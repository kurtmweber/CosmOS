//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/i386/sound/tone.h>
#include <sys/i386/sound/wav.h>
#include <tests/dev/testsb16.h>

void playsb16() {
    // get the sb
    struct device* dsp = devicemgr_find_device("dsp0");
    if (0 != dsp) {
        struct wav_header* wav = sound_get_tone();

        struct deviceapi_dsp* dsp_api = (struct deviceapi_dsp*)dsp->api;
        dsp_play_function play_func = dsp_api->play;
        uint8_t* pcmstart = wav_pcm_start(wav);

        (*play_func)(dsp, pcmstart, wav->sample_rate, wav->bit_depth, wav->num_channels, wav->data_bytes);
    } else {
        kprintf("Unable to find dsp0\n");
    }
}
