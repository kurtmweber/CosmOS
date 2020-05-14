/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _STRING_H
#define _STRING_H

#include <types.h>

#define UINT64_DECIMAL_STRING_LENGTH 21

#ifndef _ITOA_C
void uitoa3(uint64_t n, char *s, uint8_t len);
#endif

#endif