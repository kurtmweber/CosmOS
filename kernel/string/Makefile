include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: string.a

string.a: itoa.o string.o
	$(AR) $(ARFLAGS) $@ $^
	
itoa.o: itoa.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
string.o: string.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean: string-a-clean itoa-o-clean string-o-clean

string-a-clean:
	$(RM) string.a

itoa-o-clean:
	$(RM) itoa.o
	
string-o-clean:
	$(RM) string.o