include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crsT

archive: vga-dir video.a

video.a: text.o video.o vga/vga.a
	$(AR) $(ARFLAGS) $@ $^

text.o: text.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
video.o: video.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
vga-dir:
	cd vga && $(MAKE)
	
clean: vga-dir-clean
	$(RM) *.o
	$(RM) *.a
	
vga-dir-clean:
	cd vga && $(MAKE) clean