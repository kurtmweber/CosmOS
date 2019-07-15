NASM=nasm -O0 -f bin
RM=rm -f

all: boot

boot: boot.bin boot2.bin

boot.bin: boot.asm
	$(NASM) boot.asm -o boot.bin
	
boot2.bin: boot2.asm
	$(NASM) boot2.asm -o boot2.bin
	
clean: boot-clean

boot-clean: boot-bin-clean boot2-bin-clean

boot-bin-clean:
	$(RM) boot.bin

boot2-bin-clean:
	$(RM) boot2.bin