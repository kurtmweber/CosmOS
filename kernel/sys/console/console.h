/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <types.h>

#define CONSOLE_TAB_WIDTH 5

uint64_t kprintf(const char *s, ...);
uint64_t kprintf_proc_format_string(const char *s, uint64_t *chars_written, __builtin_va_list ap);

#endif