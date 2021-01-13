//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ASSERT_H
#define _ASSERT_H

#include <sys/panic/panic.h>

// assertion macros to make life easier
// in a RELEASE build these can just expand to nothing
#define ASSERT(condition, message) if (false==(condition)) {panic (message);}
#define ASSERT_NOT_NULL(term) if (0==(term)) {panic ("Illegal null");}

#endif
