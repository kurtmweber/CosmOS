/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>
#include <types.h>

mem_block *find_containing_block(void *addr, mem_block *list) {
    ASSERT_NOT_NULL(addr);

    // returns NULL if the requested address is not found within a block in the list
    mem_block *tmp;

    tmp = list;

    do {
        if ((addr >= tmp->base) && (addr <= (tmp->base + tmp->len - 1))) {
            return tmp;
        }

    } while (tmp = tmp->next);

    return NULL;
}

const uint16_t map_base_addr = 0x500;

int_15_map *read_int_15_map(uint8_t *num_blocks, uint8_t *lrg_block) {
    ASSERT_NOT_NULL(num_blocks);
    ASSERT_NOT_NULL(lrg_block);

    int_15_map *map;
    uint8_t i = 0;

    map = (int_15_map *)((uint64_t)map_base_addr);

    *lrg_block = 0;  // don't assume it's initialized by caller

    // boot loader places a block of all zeroes after the last memory block
    // so as long as at least one field is nonzero, it's a legit block
    // once we reach all zeroes, we know we're at the end
    while (map[i].base || map[i].len || map[i].type || map[i].acpi) {
        if (map[i].type == USABLE) {
            if (map[i].len > map[*lrg_block].len) {
                *lrg_block = i;
            }
        }
        i++;
    }

    *num_blocks = i;

    return map;
}