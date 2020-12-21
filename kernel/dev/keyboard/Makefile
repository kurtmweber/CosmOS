include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ISRCFLAGS=-mgeneral-regs-only
ARFLAGS=-crs

all: keyboard.a

keyboard.a: keyboard.o keyboardbuffer.o abstract_keyboard.o
	$(AR) $(ARFLAGS) $@ $^
	
keyboard.o: keyboard.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^
	
keyboardbuffer.o: keyboardbuffer.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^
	
abstract_keyboard.o: abstract_keyboard.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a