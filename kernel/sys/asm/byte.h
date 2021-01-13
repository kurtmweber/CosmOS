//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __BYTE_H
#define __BYTE_H

#define LOW_OF_W(x) (short)(x & 0x00FF)
#define HIGH_OF_W(x) (short)((x & 0xFF00) >> 8)

#endif