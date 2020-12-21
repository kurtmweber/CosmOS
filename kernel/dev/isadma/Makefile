include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: isadma.a

isadma.a: isadma.o 
	$(AR) $(ARFLAGS) $@ $^
	
isadma.o: isadma.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a