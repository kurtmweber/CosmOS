#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# build triplet is machine-vendor-operatingsystem

# cc (clang toolchain)
CC=cc
CFLAGS=-c -target x86_64-none-elf -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large 

# ld
LD=x86_64-elf-ld 
LDFLAGS=-m elf_x86_64 -T cosmos.ld -Map cosmos.map -nostdlib

# objcopy
OBJCOPY=x86_64-elf-objcopy
OBJCOPYFLAGS=-O binary

# ar
AR=x86_64-elf-ar
ARFLAGS=-crs

