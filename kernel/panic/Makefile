include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: panic.a

panic.a: panic.o
	$(AR) $(ARFLAGS) $@ $^
	
panic.o: panic.c panic.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean: panic-a-clean panic-o-clean

panic-a-clean:
	$(RM) panic.a
	
panic-o-clean:
	$(RM) panic.o