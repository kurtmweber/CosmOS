/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PAGETABLES_H
#define _PAGETABLES_H

#include <types.h>

typedef uint64_t ptt_t;     // page translation table

#define PTT_EXTRACT_BASE(x) (x & 0x000FFFFFFFFFF000)
#define PTT_SET_BASE(ptt, base) (ptt |= base)

#endif