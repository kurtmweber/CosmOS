#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020 Tom Everett                                 *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

## generic instructions for building an archive
## everything below this point is the same for every archive.....

ARFLAGS=-crs
SRC_DIR=.

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c' | sort)
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h' | sort)
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm' | sort)

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

# this finds the names of all subdirectories under this one, that contain a Makefile
SUBDIRS=$(shell find ./* -mindepth 1 -type f -name 'Makefile'| awk -F '/' '{print $$2}' | uniq | sort) 
#$(info $$SUBDIRS is [${SUBDIRS}])

# build a list of directories to archives
ARCHIVE_DIRS=$(addsuffix /, $(SUBDIRS))
#$(info $$ARCHIVE_DIRS is [${ARCHIVE_DIRS}])

# make the archive names
ARCHIVE_NAMES=$(addsuffix .a, $(SUBDIRS))
#$(info $$ARCHIVE_NAMES is [${ARCHIVE_NAMES}])

# make a list of paths to archives
ARCHIVES=$(join $(ARCHIVE_DIRS),$(ARCHIVE_NAMES))
#$(info $$ARCHIVES is [${ARCHIVES}])

# set search paths
vpath %.c .
vpath %.h .
vpath %.asm .

archive: $(ARCHIVE_FILE)

$(ARCHIVE_FILE): $(ARCHIVES) $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) -f elf64 $< -o $@ 

# archives
$(ARCHIVES):
	$(foreach file, $(SUBDIRS), cd $(file) && make archive && cd ..;)

clean: clean-subprojects
	$(RM) $(OBJS)
	$(RM) $(ARCHIVE_FILE)

clean-subprojects:
	$(foreach file, $(SUBDIRS), cd $(file) && make clean && cd ..;)