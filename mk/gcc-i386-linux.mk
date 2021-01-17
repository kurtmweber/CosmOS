#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# platform flags
COMPILE_PLATFORM=COMPILE_PLATFORM_LINUX
TARGET_PLATFORM=TARGET_PLATFORM_i386

# cc (GCC tool chain)
CC=gcc
CFLAGS=-c -Werror -m64 -mno-red-zone -ffreestanding -fPIC -O0 -D$(COMPILE_PLATFORM) -D$(TARGET_PLATFORM) -g3

# ld
LD=ld
LDFLAGS=-m elf_x86_64 -T $(LINKER_SCRIPT) -Map $(MAPFILE) -nostdlib --no-relax

# objcopy
OBJCOPY=objcopy
OBJCOPYFLAGS=-O binary

# ar
AR=ar
ARFLAGS=-crs
