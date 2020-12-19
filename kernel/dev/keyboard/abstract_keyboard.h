/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ABSTRACTKEYBOARD_H
#define _ABSTRACTKEYBOARD_H

#include <types.h>

// Macros for keyboard operations
#define P(row, col)	((row << 5) + col)

extern const uint8_t abstract_keyboard_grid[256];
extern const uint8_t abstract_keyboard_grid_2byte[256];

#endif