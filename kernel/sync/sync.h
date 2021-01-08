/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>

typedef volatile bool kernel_spinlock;    // 64 bytes to take up a full cache line, which improves performance on atomic operations


// spinlock.c
extern kernel_spinlock mem_lock;

void spinlocks_init();
void spinlock_acquire(kernel_spinlock *lock);
void spinlock_release(kernel_spinlock *lock);