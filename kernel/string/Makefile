include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crs

all: string.a

string.a: itoa.o string.o
	$(AR) $(ARFLAGS) $@ $^
	
itoa.o: itoa.c
	$(CC) $(CFLAGS) -o $@ $^
	
string.o: string.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: string-a-clean itoa-o-clean string-o-clean

string-a-clean:
	$(RM) string.a

itoa-o-clean:
	$(RM) itoa.o
	
string-o-clean:
	$(RM) string.o