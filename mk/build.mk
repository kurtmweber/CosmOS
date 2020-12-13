
NASM=nasm -f bin
RM=rm -f
MAKE=make

# cc
CC=gcc-10
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large 

# ar
AR=ar

# ld
LD=ld
#LDFLAGS=-e CosmOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib
LDFLAGS=-T cosmos.ld -Map cosmos.map


