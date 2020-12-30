//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _WAV_H
#define _WAV_H

#include <types.h>

struct wav_header {
    // "RIFF"
    uint8_t riff_header[4];
    // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    uint32_t wav_size; 
    // "WAVE"
    uint8_t wave_header[4]; 
    
    // "fmt "
    uint8_t fmt_header[4];
    // Should be 16 for PCM
    uint32_t fmt_chunk_size; 
    // Should be 1 for PCM. 3 for IEEE Float
    uint16_t audio_format; 
    uint16_t num_channels;
    uint32_t sample_rate;
     // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    uint32_t byte_rate;
    // num_channels * Bytes Per Sample
    uint16_t sample_alignment; 
    // Number of bits per sample
    uint16_t bit_depth; 
    
    // "data"
    uint8_t data_header[4]; // Contains "data"
    // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t data_bytes; 
};

uint8_t* wav_pcm_start(struct wav_header* header);

#endif