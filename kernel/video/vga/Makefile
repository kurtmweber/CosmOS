include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: vga.a

vga.a: cursor.o mode.o text.o vga.o
	$(AR) $(ARFLAGS) $@ $^
	
cursor.o: cursor.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

mode.o: mode.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
text.o: text.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
vga.o: vga.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean: 
	$(RM) *.o
	$(RM) *.a