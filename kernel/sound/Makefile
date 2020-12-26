include ../../mk/build.mk

ARFLAGS=-crs
EXTRA_CFLAGS=-I..

all: tone.o
	
tone.o: tone.asm
	$(NASM) $(NASMARGS) -f elf64 tone.asm -o tone.o

clean: 
	$(RM) *.o
	$(RM) *.a
