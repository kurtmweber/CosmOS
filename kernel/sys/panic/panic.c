/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/serial/serial.h>
#include <sys/asm/asm.h>

void panic(const char* s) {
    serial_write_string("!!!PANIC!!!\n");
    serial_write_string(s);

    asm_cli();
    asm_hlt();
}
