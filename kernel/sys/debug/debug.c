//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>

// normally this including from /dev is not encouraged and instead the devivemgr should be used
// however, for debug, this might actually be a better approach
#include <dev/i386/serial/com1.h>

#define DEBUG_LINE_WIDTH 32
#define BYTE_WIDTH 5  // "0x" plus 2 nybbles, plus a space

/*
 * (width_of_hex + width_of_byte_count + width_of_ascii)*num_ytes + extra
 */
#define LINE_WIDTH ((DEBUG_LINE_WIDTH + DEBUG_LINE_WIDTH + DEBUG_LINE_WIDTH) * BYTE_WIDTH) + 128
/*
 * ASCII
 */
#define ASCII_MIN 0x21
#define ASCII_MAX 0x7e
/*
 * lookup table
 */
uint8_t CHAR_ARRAY[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void debug_show_memblock(uint8_t* block, uint16_t size) {
    uint8_t out_bytes[LINE_WIDTH];
    uint8_t index_string[32];
    uint16_t line_count = 0;
    uint64_t i = 0;

    /*
     * index
     */
    uitoa3(i, index_string, 32, 16);
    for (uint8_t k = 0; k < strlen(index_string); k++) {
        out_bytes[line_count++] = index_string[k];
    }
    out_bytes[line_count++] = ':';
    out_bytes[line_count++] = ' ';

    while (i < size) {
        /*
         * hex bytes
         */
        out_bytes[line_count++] = '0';
        out_bytes[line_count++] = 'x';
        uint8_t byte = block[i];
        uint8_t high = (byte & 0xF0) >> 4;
        out_bytes[line_count++] = CHAR_ARRAY[high];
        uint8_t low = byte & 0x0F;
        out_bytes[line_count++] = CHAR_ARRAY[low];
        out_bytes[line_count++] = ' ';

        if (0 == ((i + 1) % (uint8_t)DEBUG_LINE_WIDTH)) {
            /*
             * ok we added the hex bytes, now add the ascii
             */
            out_bytes[line_count++] = ' ';
            for (uint16_t i = 0; i < DEBUG_LINE_WIDTH; i++) {
                if ((block[i] >= ASCII_MIN) && (block[i] <= ASCII_MAX)) {
                    out_bytes[line_count++] = block[i];
                } else {
                    out_bytes[line_count++] = '.';
                }
            }
            /*
             * write the line
             */
            out_bytes[line_count++] = 0;
            com1_serial_write(out_bytes);
            /*
             * make sure we didnt index over
             */
            if (line_count > LINE_WIDTH) {
                panic("over-write in debug_show_memblock");
            }
            /*
             * EOL
             */
            com1_serial_write("\n");
            /*
             * reset line count
             */
            line_count = 0;
            /*
             * next index
             */
            uitoa3(i, index_string, 32, 16);

            /*
             * index
             */
            for (uint8_t k = 0; k < strlen(index_string); k++) {
                out_bytes[line_count++] = index_string[k];
            }
            out_bytes[line_count++] = ':';
            out_bytes[line_count++] = ' ';
        }
        /*
         * we processed a byte
         */
        i = i + 1;
    }
}
