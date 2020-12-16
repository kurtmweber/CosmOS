include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: array.a

array.a: array.o 
	$(AR) $(ARFLAGS) $@ $^
	
array.o: array.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a