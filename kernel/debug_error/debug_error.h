/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DEBUG_ERROR_H
#define _DEBUG_ERROR_H

#include <types.h>

#ifndef _DEBUGINT_C
void debug_print_uint64(uint64_t n);
#endif

#ifndef _PANIC_C
void panic(const char *s);
#endif

#endif