#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# brew install llvm

# clang root
CLANG_ROOT=/usr/local/opt/llvm/bin/

# platform flag
PLATFORM=COMPILE_PLATFORM_DARWIN

# cc (GCC tool chain)
CC=clang
CFLAGS=-c -target x86_64-unknown-elf -m64 -mno-red-zone -ffreestanding -fPIC -O0 -D$(PLATFORM)

# ld
LD=x86_64-elf-ld 
LDFLAGS=-m elf_x86_64 -T $(LINKER_SCRIPT) -Map $(MAPFILE) -nostdlib --no-relax

# objcopy
OBJCOPY=x86_64-elf-objcopy
OBJCOPYFLAGS=-O binary

# ar
AR=x86_64-elf-ar
ARFLAGS=-crs
