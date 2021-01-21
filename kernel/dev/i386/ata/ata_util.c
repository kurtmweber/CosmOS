/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/ata/ata_controller.h>
#include <dev/i386/ata/ata_util.h>
#include <sys/asm/asm.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/sleep/sleep.h>

//#define ATA_UTIL_DEBUG

uint8_t ata_register_read(struct ata_controller* controller, uint8_t channel, ata_registers reg) {
    uint8_t ret = asm_in_b(ata_register_port_number(controller, channel, reg));
#ifdef ATA_UTIL_DEBUG
    kprintf("ata_register_read %#llX from %#llX\n", ret, ata_register_port_number(controller, channel, reg));
#endif
    return ret;
}

uint32_t ata_register_read_dword(struct ata_controller* controller, uint8_t channel, ata_registers reg) {
    return asm_in_d(ata_register_port_number(controller, channel, reg));
}

uint16_t ata_register_read_word(struct ata_controller* controller, uint8_t channel, ata_registers reg) {
    uint16_t ret = asm_in_w(ata_register_port_number(controller, channel, reg));
#ifdef ATA_UTIL_DEBUG
    kprintf("ata_register_read %#llX from %#llX\n", ret, ata_register_port_number(controller, channel, reg));
#endif
    return ret;
}

void ata_register_write(struct ata_controller* controller, uint8_t channel, ata_registers reg, uint8_t value) {
#ifdef ATA_UTIL_DEBUG
    kprintf("ata_register_write %#llX to %#llX\n", value, ata_register_port_number(controller, channel, reg));
#endif
    asm_out_b(ata_register_port_number(controller, channel, reg), value);
    return;
}

void ata_register_write_word(struct ata_controller* controller, uint8_t channel, ata_registers reg, uint16_t value) {
#ifdef ATA_UTIL_DEBUG
    kprintf("ata_register_write %#llX to %#llX\n", value, ata_register_port_number(controller, channel, reg));
#endif
    asm_out_w(ata_register_port_number(controller, channel, reg), value);
    return;
}
/*
 * calculate port offste
 */
uint16_t ata_register_port_number(struct ata_controller* controller, uint8_t channel, ata_registers reg) {
    uint16_t port_prim_base = 0, port_prim_ctrl = 0, port_sec_base = 0, port_sec_ctrl = 0;
    uint16_t port_base = 0, port_ctrl = 0;
    uint16_t out_port_base = 0, out_offset = 0;

    port_prim_base = controller->channels[ATA_PRIMARY].base_io;
    port_prim_ctrl = controller->channels[ATA_PRIMARY].base_io_ctrl;
    port_sec_base = controller->channels[ATA_SECONDARY].base_io;
    port_sec_ctrl = controller->channels[ATA_SECONDARY].base_io_ctrl;

    if (channel == ATA_PRIMARY) {
        port_base = port_prim_base;
        port_ctrl = port_prim_ctrl;
    } else {
        port_base = port_sec_base;
        port_ctrl = port_sec_ctrl;
    }

    switch (reg) {
        // fallthrough is desired behavior
        case ATA_REGISTER_DATA:
        case ATA_REGISTER_ERROR:
            //	case ATA_REGISTER_FEATURES:
        case ATA_REGISTER_SECTOR_COUNT_0:
        case ATA_REGISTER_LBA_0:
        case ATA_REGISTER_LBA_1:
        case ATA_REGISTER_LBA_2:
        case ATA_REGISTER_HDDEVSEL:
        case ATA_REGISTER_COMMAND:
            //	case ATA_REGISTER_STATUS:
        case ATA_REGISTER_SECTOR_COUNT_1:
        case ATA_REGISTER_LBA_3:
        case ATA_REGISTER_LBA_4:
        case ATA_REGISTER_LBA_5:
            out_port_base = port_base;
            break;
        case ATA_REGISTER_CONTROL:
            //	case ATA_REGISTER_ALT_STATUS:
        case ATA_REGISTER_DEVADDRESS:
            out_port_base = port_ctrl;
            break;
        default:
            panic("Invalid or unsupported ATA register selected!");
            break;
    }

    switch (reg) {
        // again, fallthrough is intentional
        case ATA_REGISTER_DATA:
            out_offset = 0;
            break;
        case ATA_REGISTER_ERROR:
            //	case ATA_REGISTER_FEATURES:
            out_offset = 1;
            break;
        case ATA_REGISTER_SECTOR_COUNT_0:
        case ATA_REGISTER_SECTOR_COUNT_1:
            out_offset = 2;
            break;
        case ATA_REGISTER_LBA_0:
        case ATA_REGISTER_LBA_3:
            out_offset = 3;
            break;
        case ATA_REGISTER_LBA_1:
        case ATA_REGISTER_LBA_4:
            out_offset = 4;
            break;
        case ATA_REGISTER_LBA_2:
        case ATA_REGISTER_LBA_5:
            out_offset = 5;
            break;
        case ATA_REGISTER_HDDEVSEL:
            out_offset = 6;
            break;
        case ATA_REGISTER_COMMAND:
            //	case ATA_REGISTER_STATUS:
            out_offset = 7;
            break;
        case ATA_REGISTER_CONTROL:
            //	case ATA_REGISTER_ALT_STATUS:
            out_offset = 0;
            break;
        case ATA_REGISTER_DEVADDRESS:
            out_offset = 1;
            break;
        default:
            panic("Invalid or unsupported ATA register selected!");
            break;
    }
    return out_offset + out_port_base;
}

void ata_detect_atapi(struct ata_controller* controller, uint8_t channel) {
    ata_register_write(controller, channel, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY_PACKET);
    sleep_wait(1);
    return;
}

bool ata_channel_ready(struct ata_controller* controller, uint8_t channel) {
    BYTE status;

    status = ata_register_read(controller, channel, ATA_REGISTER_ALT_STATUS);
    if (status & ATA_STATUS_BUSY) {  // BSY set (bit 7)
        return false;
    }

    if ((status & ATA_STATUS_ERROR) || (status & ATA_STATUS_WRITE_FAULT)) {
        return false;
    }

    if (status & ATA_STATUS_DRIVE_READY) {
        return true;
    }

    return false;
}

bool ata_select_device(struct ata_controller* controller, uint8_t channel, uint8_t device) {
    BYTE status;

    if (controller->channels[channel].selected_device == device) {
        return true;
    }

    status = ata_register_read(controller, channel, ATA_REGISTER_STATUS);
    // bit 7 of status register indicates busy--if it's set then nothing else matters, if not then we check bit 3
    // which indicates ready for data, and we can't change the device on the channel if the already-selected device is waiting for data
    if ((status & ATA_STATUS_BUSY) || (status & ATA_STATUS_DRQ)) {
        return false;
    }

    controller->channels[channel].selected_device = device;

    ata_register_write(controller, channel, ATA_REGISTER_HDDEVSEL, 0xA0 | (device << 4));
    sleep_wait(1);

    return true;
}

void ata_interrupt_enable(struct ata_controller* controller, uint8_t channel, bool enabled) {
    /*
     * to clarify, because this may look backwards:
     * yes, the correct thing to do is to clear bit 1 to enable IRQs,
     * and set it to disable
     */
    // bit 3 is specified as reserved and set to 1, so we have to make sure we don't zero it
    ata_register_write(controller, channel, ATA_REGISTER_CONTROL, 0x08 | (enabled ? 0 : 2));
    return;
}

void ata_wait_busy(struct ata_controller* controller, uint8_t channel) {
    while (ata_register_read(controller, channel, ATA_REGISTER_STATUS) & ATA_STATUS_BUSY) {
    };
}
void ata_wait_drq(struct ata_controller* controller, uint8_t channel) {
    while (!ata_register_read(controller, channel, ATA_REGISTER_STATUS) & ATA_STATUS_DRQ) {
    };
}