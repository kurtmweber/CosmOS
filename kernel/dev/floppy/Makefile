include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: floppy.a

floppy.a: floppy.o 
	$(AR) $(ARFLAGS) $@ $^
	
floppy.o: floppy.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a