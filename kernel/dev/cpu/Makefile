include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: cpu.a

cpu.a: cpu.o 
	$(AR) $(ARFLAGS) $@ $^
	
cpu.o: cpu.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a