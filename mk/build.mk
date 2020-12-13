
NASM=nasm -f bin
RM=rm -f
MAKE=make

# cc
CC=gcc

# ar
AR=ar

# ld
LD=ld
#LDFLAGS=-e CosmOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib
LDFLAGS=-T cosmos.ld -Map cosmos.map


