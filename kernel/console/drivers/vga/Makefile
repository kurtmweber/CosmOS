include ../../../../mk/build.mk

EXTRA_CFLAGS=-I../../..
ARFLAGS=-crs

all: vga.a

vga.a: vga_console.o vga_console_write.o
	$(AR) $(ARFLAGS) $@ $^
	
vga_console.o: vga_console.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

vga_console_write.o: vga_console_write.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean: vga-a-clean vga_console-o-clean vga_console_write-o-clean

vga_console-o-clean:
	$(RM) vga_console.o
	
vga_console_write-o-clean:
	$(RM) vga_console_write.o
	
vga-a-clean:
	$(RM) vga.a