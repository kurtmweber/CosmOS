include ../scripts/mk/build.mk

ARCHIVE_FILE=kernel.a
KERNEL_ELF=cosmos.elf
KERNEL_BIN=cosmos.bin

EXTRA_CFLAGS= -I.

SRC_DIR=.

ifeq ($(TARGET_PLATFORM),TARGET_PLATFORM_i386)
IGNORE_DIRS=/arm/
else
IGNORE_DIRS=/i386/
endif

all: $(KERNEL_BIN)

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c' | grep -v "$(IGNORE_DIRS)" | sort)
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h' | grep -v "$(IGNORE_DIRS)" | sort)
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm' | grep -v "$(IGNORE_DIRS)" | sort)

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

# set search paths
vpath %.c .
vpath %.h .
vpath %.asm .

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)

$(KERNEL_ELF): $(ARCHIVE_FILE)
ifeq ($(TARGET_PLATFORM),TARGET_PLATFORM_i386)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ARCHIVE_FILE) sys/i386/rawdata/rawdata.o
else
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ARCHIVE_FILE)
endif

$(ARCHIVE_FILE): $(ARCHIVES) $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) $< -o $@ 

clean:
	$(RM) $(OBJS)
	$(RM) $(ARCHIVE_FILE)
	$(RM) $(KERNEL_ELF)
	$(RM) $(KERNEL_BIN)
