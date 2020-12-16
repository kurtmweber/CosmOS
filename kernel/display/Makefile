include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: display.a

display.a: display.o 
	$(AR) $(ARFLAGS) $@ $^
	
display.o: display.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a