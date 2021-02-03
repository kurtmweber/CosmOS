//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/bdos/bdos.h>

void bdos_system_reset() {}
uint8_t bdos_console_input() {
    return 0;
}
void bdos_console_output(uint8_t c) {}

void bdos_select_disk(uint8_t* name) {}
uint8_t bdos_open_file(struct bdos_fcb fcb) {
    return 0;
}
uint8_t bdos_close_file(struct bdos_fcb fcb) {
    return 0;
}
uint8_t bdos_delete_file(struct bdos_fcb fcb) {
    return 0;
}
uint8_t bdos_make_file(struct bdos_fcb fcb) {
    return 0;
}
uint8_t bdos_rename_file(struct bdos_fcb fcb) {
    return 0;
}