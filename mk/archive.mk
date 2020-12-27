
## generic instructions for building an archive
## everything below this point is the same for every archive.....

ARFLAGS=-crs
SRC_DIR=.

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h')
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm')

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

# set search paths
vpath %.c .
vpath %.h .
vpath %.asm .

all: $(ARCHIVE_FILE)

$(ARCHIVE_FILE): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)
	$(RM) $(ARCHIVE_FILE)
