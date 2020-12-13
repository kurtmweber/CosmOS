include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crsT

all: vga-dir video.a

video.a: text.o video.o vga/vga.a
	$(AR) $(ARFLAGS) $@ $^

text.o: text.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
video.o: video.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
vga-dir:
	cd vga && $(MAKE)
	
clean: vga-dir-clean video-a-clean text-o-clean video-o-clean

video-a-clean:
	$(RM) video.a
	
text-o-clean:
	$(RM) text.o

video-o-clean:
	$(RM) video.o
	
vga-dir-clean:
	cd vga && $(MAKE) clean