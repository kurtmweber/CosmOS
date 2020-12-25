/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PANIC_H
#define _PANIC_H

// assertion macros to make life easier
// in a RELEASE build these can just expand to nothing
#define ASSERT(condition, message) if (false==condition) {panic (message);}
#define ASSERT_NOT_NULL(term, message) if (0==term) {panic (message);}

void panic(const char *s);

#endif