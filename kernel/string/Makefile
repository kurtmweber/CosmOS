include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: string.a

string.a: itoa.o memops.o string.o
	$(AR) $(ARFLAGS) $@ $^
	
itoa.o: itoa.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

memops.o: memops.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
string.o: string.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a