
NASM=nasm -f bin
RM=rm -f
MAKE=make

# detect build platform
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
# cc
CC=cc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large 
else
# cc
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large 
endif

# ar
AR=ar

# ld
LD=ld
#LDFLAGS=-e CosmOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib
LDFLAGS=-T cosmos.ld -Map cosmos.map


