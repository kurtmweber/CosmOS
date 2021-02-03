/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KPRINTF_C
#define _KPRINTF_C

#include <dev/x86-64/serial/serial.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <types.h>

uint64_t kprintf_proc_format_string(const char* s, uint64_t* chars_written, __builtin_va_list ap);

uint64_t kprintf(const char* s, ...) {
    __builtin_va_list ap;
    uint64_t idx_in = 0, idx_out = 0;
    uint64_t chars_written = 0;
    char out_str[64] = {'\0'};

    __builtin_va_start(ap, s);

    while (s[idx_in]) {
        if (s[idx_in] != '%') {
            out_str[idx_out] = s[idx_in];
            idx_out++;
        } else {
            switch (s[idx_in + 1]) {
                case '%':
                    out_str[idx_out] = '%';
                    idx_out++;
                    idx_in++;
                    break;
                default:
                    chars_written += idx_out;
                    out_str[idx_out] = '\0';
                    idx_out = 0;
                    serial_write_string(out_str);

                    idx_in += kprintf_proc_format_string(&s[idx_in + 1], &chars_written, ap);
                    break;
            }
        }

        idx_in++;

        if (idx_out == 63) {
            out_str[63] = '\0';
            serial_write_string(out_str);
            idx_out = 0;
            chars_written += 63;
        }
    }

    __builtin_va_end(ap);

    chars_written += idx_out;
    out_str[idx_out] = '\0';
    serial_write_string(out_str);

    return chars_written;
}

uint64_t kprintf_do_uint(uint8_t width, char* output, uint8_t base, __builtin_va_list ap) {
    // conversion variables
    unsigned int conv_uint;
    unsigned long conv_ulong;

    switch (width) {
        // fallthrough is intentional, as 8-bit and 16-bit unsigned ints are promoted by compiler to 32-bit unsigned int
        case 8:
        case 16:
        case 32:
            conv_uint = __builtin_va_arg(ap, unsigned int);
            uitoa3(conv_uint, output, 66, base);
            break;
        case 64:
            conv_ulong = __builtin_va_arg(ap, unsigned long);
            uitoa3(conv_ulong, output, 66, base);
            break;
    }
    serial_write_string(output);
    return strlen(output);
}

uint64_t kprintf_proc_format_string(const char* s, uint64_t* chars_written, __builtin_va_list ap) {
    // maximum length of an output string is 66 bytes:
    // 64 binary digits (for an unsigned 64-bit type) or 63 and a +/- sign (if signed)
    // possible "b" sigil at end
    // NULL terminator
    char output[66];
    uint64_t i = 0;
    uint64_t consumed = 0;
    uint8_t width = 0;

    bool hex_prefix = false;

    // conversion variables
    const char* conv_str;

    while (s[i]) {
        consumed++;
        switch (s[i++]) {
            case 'c':
                output[0] = __builtin_va_arg(ap, int);
                output[1] = '\0';
                serial_write_string(output);
                (*chars_written)++;
                return consumed;
            case 'h':
                width = 8;
                break;
            case 'l':
                if (width == 32) {
                    width = 64;
                } else {
                    width = 32;
                }
                break;
            case 's':
                conv_str = __builtin_va_arg(ap, const char*);
                serial_write_string(conv_str);
                *chars_written += strlen(conv_str);
                return consumed;
            case 'u':
                if (!width) {
                    width = 16;
                }
                *chars_written += kprintf_do_uint(width, output, 10, ap);
                return consumed;
            case 'X':
                if (!width) {
                    width = 16;
                }
                if (hex_prefix) {
                    serial_write_string("0x");
                    *chars_written += 2;
                }

                *chars_written += kprintf_do_uint(width, output, 16, ap);
                return consumed;
            case '#':
                hex_prefix = true;
                break;
            default:
                return consumed;
        }
    }
    return 0;
}

#endif