include ../mk/build.mk

ARCHIVE_FILE=kernel.a
KERNEL_ELF=cosmos.elf
KERNEL_BIN=cosmos.bin

include ../mk/archive.mk

EXTRA_CFLAGS= -I.

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)

$(KERNEL_ELF): $(ARCHIVE_FILE)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ARCHIVE_FILE) rawdata/rawdata.o
