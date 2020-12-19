include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: ethernet.a

ethernet.a: ethernet.o 
	$(AR) $(ARFLAGS) $@ $^
	
ethernet.o: ethernet.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a