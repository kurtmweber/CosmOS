include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: cmos.a

cmos.a: cmos.o 
	$(AR) $(ARFLAGS) $@ $^
	
cmos.o: cmos.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a