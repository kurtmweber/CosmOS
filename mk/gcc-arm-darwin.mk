#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# platform flag
COMPILE_PLATFORM=COMPILE_PLATFORM_DARWIN
TARGET_PLATFORM=TARGET_PLATFORM_ARM

# cc (GCC tool chain)
CC=arm-none-eabi-gcc
CFLAGS=-c -ffreestanding -fPIC -O0 -D$(COMPILE_PLATFORM) -D$(TARGET_PLATFORM)

# ld
LD=arm-none-eabi-ld
LDFLAGS=-m elf_x86_64 -T $(LINKER_SCRIPT) -Map $(MAPFILE) -nostdlib --no-relax

# objcopy
OBJCOPY=arm-none-eabi-objcopy
OBJCOPYFLAGS=-O binary

# ar
AR=arm-none-eabi-gcc-ar
ARFLAGS=-crs
