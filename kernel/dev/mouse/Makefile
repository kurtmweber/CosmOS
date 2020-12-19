include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: mouse.a

mouse.a: mouse.o 
	$(AR) $(ARFLAGS) $@ $^
	
mouse.o: mouse.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a