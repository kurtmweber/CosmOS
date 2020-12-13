include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crsT

all: vga-dir video.a

video.a: text.o video.o vga/vga.a
	$(AR) $(ARFLAGS) $@ $^

text.o: text.c
	$(CC) $(CFLAGS) -o $@ $^
	
video.o: video.c
	$(CC) $(CFLAGS) -o $@ $^
	
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