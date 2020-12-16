include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: serial.a

serial.a: serial.o 
	$(AR) $(ARFLAGS) $@ $^
	
serial.o: serial.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a