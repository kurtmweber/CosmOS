//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TONE_H
#define _TONE_H

#include <types.h>
#include <sys/i386/sound/wav.h>

struct wav_header* sound_get_tone();

#endif