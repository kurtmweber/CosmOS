/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <sync/sync.h>

kernel_spinlock mem_lock;

void spinlocks_init(){
    mem_lock = false;

    return;
}

void spinlock_acquire(kernel_spinlock *lock){
    bool expected = false;
    bool desired = true;

    while(!__atomic_compare_exchange(lock, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)){
        /*
         * Per GCC documentation:
         * bool __atomic_compare_exchange_n (type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
         * "If [*ptr and *expected] are not equal, the operation is a read and
         * the current contents of *ptr are written into *expected." Thus, the
         * comparison will succeed the next time through, so we have to reset
         * expected to properly spin.
         */
        expected = false;
    }

    return;
}

void spinlock_release(kernel_spinlock *lock){
    *lock = false;

    return;
}