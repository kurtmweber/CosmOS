/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>
#include <types.h>

#define MALLOC_MAGIC_0 'C'
#define MALLOC_MAGIC_1 'O'
#define MALLOC_MAGIC_2 'S'
#define MALLOC_MAGIC_3 'M'
#define MALLOC_MAGIC_4 'O'
#define MALLOC_MAGIC_5 'S'

kmalloc_block *kmalloc_block_list;
kmalloc_block *kmalloc_block_list_end;

uint8_t kmalloc_block_valid(kmalloc_block *b) {
    ASSERT_NOT_NULL(b);
    if (b->start_magic[0] != MALLOC_MAGIC_0) {
        return false;
    }
    if (b->start_magic[1] != MALLOC_MAGIC_1) {
        return false;
    }
    if (b->start_magic[2] != MALLOC_MAGIC_2) {
        return false;
    }
    if (b->start_magic[3] != MALLOC_MAGIC_3) {
        return false;
    }
    if (b->start_magic[4] != MALLOC_MAGIC_4) {
        return false;
    }
    if (b->start_magic[5] != MALLOC_MAGIC_5) {
        return false;
    }
    if (b->end_magic[0] != MALLOC_MAGIC_5) {
        return false;
    }
    if (b->end_magic[1] != MALLOC_MAGIC_4) {
        return false;
    }
    if (b->end_magic[2] != MALLOC_MAGIC_3) {
        return false;
    }
    if (b->end_magic[3] != MALLOC_MAGIC_2) {
        return false;
    }
    if (b->end_magic[4] != MALLOC_MAGIC_1) {
        return false;
    }
    if (b->end_magic[5] != MALLOC_MAGIC_0) {
        return false;
    }

    return true;
}

kmalloc_block *kmalloc_block_from_address(void *ptr) {
    ASSERT_NOT_NULL(ptr);
    // The following is arithmetic on a void pointer, which is not permitted per C standard.
    // However, per GCC documentation, as a nonstandard extension GCC permits pointer arithmetic
    // on void pointers with an assumption that the object size is 1.
    // At time of writing (2020-05-25), this is documented at https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html
    kmalloc_block *ret = ptr - sizeof(kmalloc_block);
    ASSERT(kmalloc_block_valid(ret));
    return ret;
}

uint8_t kmalloc_pointer_valid(void *ptr) {
    ASSERT_NOT_NULL(ptr);
    kmalloc_block *block = kmalloc_block_from_address(ptr);
    ASSERT_NOT_NULL(block);
    return kmalloc_block_valid(block);
}

kmalloc_block *find_avail_kmalloc_block_list(uint64_t size) {
    ASSERT(0 != size);
    kmalloc_block *cur_block;
    kmalloc_block *best = 0;
    kmalloc_block *last = 0;
    // Find the first block on the heap suitable for the size requested.
    // If we find an unused block of exactly the size requested, return it immediately.
    // Otherwise, we return the smallest unused block that is AT LEAST big enough to
    // satisfy the request.  For now, we make no attempt to split the block--later on
    // that may be added, but for now let's keep it simple even if that results in a more
    // inefficient use of heap space (running out of address space is at any rate an unlikely
    // issue for 64-bit, though physical space is perhaps a different story)

    // If no sufficiently-sized block is found, create a new one after the last block.
    // If there is not enough address space to hold the block (including the header, return NULL

    // this shouldn't happen, because kmalloc() should initialize the list the first time it is called
    if (!kmalloc_block_list) {
        panic("Uninitialized kmalloc block list!");
    }

    cur_block = kmalloc_block_list;

    do {
        last = cur_block;  // We need to save this in case we have to add a new block.
                           // If we do, then without this we'd have to walk the list again from the beginning
                           // to find the existing last block and update its next pointer to the block we
                           // just added.
        // kprintf("about to go last: 0x%llX\n", (uint64_t)last);

        if (cur_block->used != false) {
            continue;
        }

        if (cur_block->len == size) {
            //      kprintf("same size\n");
            cur_block->used = true;
            return cur_block;
        }

        if (cur_block->len > size) {
            if (!best) {
                best = cur_block;
            } else if (best->len < cur_block->len) {
                best = cur_block;
            } else {
                continue;
            }
        }

    } while ((cur_block = cur_block->next));

    if (best) {
        //    kprintf("best\n");
        best->used = true;
        return best;
    }
    // kprintf("new_kmalloc_block\n");

    return new_kmalloc_block(last, size);
}

void kfree(void *ptr) {
    ASSERT_NOT_NULL(ptr);
    kmalloc_block *b = kmalloc_block_from_address(ptr);

    // double free check
    ASSERT(b->used = true);

    // mark not used
    b->used = false;

    if ((b->next) && (b->next->used == false)) {
        b->len = b->len + sizeof(kmalloc_block) + b->next->len;
        b->next = b->next->next;
    }

    if ((b->prev) && (b->prev->used == false)) {
        b->prev->len = b->prev->len + sizeof(kmalloc_block) + b->len;
        b->prev->next = b->next;
    }

    return;
}

void *kmalloc(uint64_t size) {
    ASSERT(0 != size);
    kmalloc_block *cur_block = 0;

    // align size to KMALLOC_ALIGN_BYTES
    if (size % KMALLOC_ALIGN_BYTES) {
        size += (KMALLOC_ALIGN_BYTES - (size % KMALLOC_ALIGN_BYTES));
    }

    // check the block list
    if (!kmalloc_block_list) {
        if ((uint64_t)brk % KMALLOC_ALIGN_BYTES) {
            cur_block = (kmalloc_block *)(brk + (KMALLOC_ALIGN_BYTES - ((uint64_t)brk % KMALLOC_ALIGN_BYTES)));
        } else {
            cur_block = (kmalloc_block *)brk;
        }
        kmalloc_block_list = cur_block;
        cur_block = new_kmalloc_block(0, size);
        ASSERT(cur_block->used == true);
    } else {
        cur_block = find_avail_kmalloc_block_list(size);
        ASSERT(cur_block->used == true);
    }

    if (!cur_block) {
        panic("Unable to return block from kmalloc");
        return 0;
    } else {
        ASSERT(kmalloc_block_valid(cur_block));
        ASSERT_NOT_NULL(cur_block->base);
        ASSERT(cur_block->used == true);
        return cur_block->base;
    }
}

void kmalloc_init() {
    kmalloc_block_list = 0;
    kmalloc_block_list_end = 0;

    return;
}

kmalloc_block *new_kmalloc_block(kmalloc_block *last, uint64_t size) {
    ASSERT(0 != size);
    /*
     * last can be null here
     */
    kmalloc_block *new = 0;

    if ((UINT64_T_MAX - (sizeof(kmalloc_block) + size - 1)) < (uint64_t)brk) {  // out of address space
        return 0;
    }

    new = brk;

    // The following is arithmetic on a void pointer, which is not permitted per C standard.
    // However, per GCC documentation, as a nonstandard extension GCC permits pointer arithmetic
    // on void pointers with an assumption that the object size is 1.
    // At time of writing (2020-05-25), this is documented at https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html
    new->base = (void *)((uint64_t) new + sizeof(kmalloc_block));
    new->start_magic[0] = MALLOC_MAGIC_0;
    new->start_magic[1] = MALLOC_MAGIC_1;
    new->start_magic[2] = MALLOC_MAGIC_2;
    new->start_magic[3] = MALLOC_MAGIC_3;
    new->start_magic[4] = MALLOC_MAGIC_4;
    new->start_magic[5] = MALLOC_MAGIC_5;

    new->end_magic[0] = MALLOC_MAGIC_5;
    new->end_magic[1] = MALLOC_MAGIC_4;
    new->end_magic[2] = MALLOC_MAGIC_3;
    new->end_magic[3] = MALLOC_MAGIC_2;
    new->end_magic[4] = MALLOC_MAGIC_1;
    new->end_magic[5] = MALLOC_MAGIC_0;

    new->len = size;
    new->used = true;
    new->next = 0;
    new->prev = last;
    new->owner = 0;

    if (last) {
        last->next = new;
    }

    // more void pointer arithmetic
    brk += (sizeof(kmalloc_block) + size);

    kmalloc_block_list_end = new;

    return new;
}

void *krealloc(void *ptr, uint64_t size) {
    ASSERT(0 != size);
    ASSERT_NOT_NULL(ptr);
    void *new_block = 0;
    BYTE *dest, *src;
    uint64_t i;

    kmalloc_block *b = kmalloc_block_from_address(ptr);
    // only realloc used blocks
    ASSERT(b->used == true);

    // to keep it simple, if the new size is less than the old size we just leave it alone
    // we might fix this later if memory pressure becomes an issues
    if (size <= b->len) {
        return ptr;
    }

    // keep everything 8-byte aligned
    if (size % KMALLOC_ALIGN_BYTES) {
        size += (KMALLOC_ALIGN_BYTES - (size % KMALLOC_ALIGN_BYTES));
    }

    // if it's the last block, then instead of creating a new block we just extend the existing one
    if (!(b->next)) {
        b->len = size;
        brk = ptr + size;
        return ptr;
    }

    // otherwise we grab a new block of the requested size, copy over the data, and free the old one
    new_block = kmalloc(size);
    src = (BYTE *)ptr;
    dest = (BYTE *)new_block;
    for (i = 0; i < b->len; i++) {
        *dest = *src;
        src++;
        dest++;
    }

    kfree(ptr);
    return new_block;
}