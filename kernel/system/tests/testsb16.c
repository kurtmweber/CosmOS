//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <system/tests/testsb16.h>
#include <system/sound/wav.h>
#include <system/devicemgr/deviceapi/deviceapi_dsp.h>
#include <system/sound/tone.h>
#include <system/console/console.h>

void playsb16() {	
	// get the sb
	struct device* dsp = devicemgr_findDevice("dsp0");
	if (0!=dsp) {
		struct wav_header* wav = sound_get_tone();

		struct deviceapi_dsp* dsp_api = (struct deviceapi_dsp*) dsp->api;
		dsp_play_function play_func = dsp_api->play;
		uint8_t* pcmstart =wav_pcm_start(wav);

		(*play_func)(dsp, pcmstart, wav->sample_rate, wav->bit_depth, wav->num_channels, wav->data_bytes);
	} else {
		kprintf("Unable to find dsp0\n");
	}
}
