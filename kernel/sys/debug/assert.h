//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ASSERT_H
#define _ASSERT_H

#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>

// assertion macros to make life easier
// in a RELEASE build these can just expand to nothing
#define ASSERT(condition)                                                                                              \
    if (false == (condition)) {                                                                                        \
        kprintf("assertion '%s' failed in file %s at line %llu\n", #condition, __FILE__, __LINE__);                    \
        panic("assertion failure");                                                                                    \
    }
#define ASSERT_NOT_NULL(term)                                                                                          \
    if (0 == (term)) {                                                                                                 \
        kprintf("'%s' cannot be null in file %s at line %llu\n", #term, __FILE__, __LINE__);                           \
        panic("assert not null failure");                                                                              \
    }

#endif
