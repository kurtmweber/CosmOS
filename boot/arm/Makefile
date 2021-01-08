
include ../../mk/build.mk

SRC_DIR=.

all: startup.a

# sources
SRCS_S=$(shell find $(SRC_DIR) -type f -iname '*.s' | sort)

# object files
OBJS_S=$(subst $(SRC_DIR)/, , $(SRCS_S:.s=.o))
OBJS=$(OBJS_S)

# set search paths
vpath %.s .

startup.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	
# s files
%.o: %.s
	$(AS) $(ASARGS) -o $@ $< 

clean:
	$(RM) *.o
	$(RM) *.a
