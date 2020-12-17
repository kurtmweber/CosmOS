#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

NASM=nasm -O0 -f bin
RM=rm -f
MAKE=make
DD=dd
MAPFILE=cosmos.map
LINKER_SCRIPT=cosmos.ld

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# detect build platform
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
include $(SELF_DIR)/gcc-darwin.mk

# not quite yet
#include $(SELF_DIR)/clang-darwin.mk
else
include $(SELF_DIR)/gcc-linux.mk
endif

# qemu
# qemu
QEMU=qemu-system-x86_64
QEMUARGS= -accel tcg,thread=single       \
  -cpu core2duo                          \
  -m 1024                                \
  -no-reboot                             \
  -drive format=raw,file=hda.img         \
  -smp 1                                 \
  -usb                                   \
  -net nic,model=ne2k_pci                \
  -vga std					                     \
  -serial stdio

  #-net nic,model=rtl8139                 \
