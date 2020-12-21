include ../../../../mk/build.mk

EXTRA_CFLAGS=-I../../..
ARFLAGS=-crs

all: serial_console.a

serial_console.a: serial_console.o
	$(AR) $(ARFLAGS) $@ $^
	
serial_console.o: serial_console.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a