#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# platform flag
PLATFORM=COMPILE_PLATFORM_LINUX

# cc (GCC tool chain)
CC=gcc-10
CFLAGS=-c -m64 -mno-red-zone -ffreestanding -fPIC -O0 -D$(PLATFORM)

# ld
LD=ld
LDFLAGS=-m elf_x86_64 -T $(LINKER_SCRIPT) -Map $(MAPFILE) -nostdlib --no-relax

# objcopy
OBJCOPY=objcopy
OBJCOPYFLAGS=-O binary

# ar
AR=ar
ARFLAGS=-crs

# qemu
QEMU=qemu-system-i386
