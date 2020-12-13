include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crs

all: ata.a

ata.a: ata.o control.o detect.o registers.o transfer.o
	$(AR) $(ARFLAGS) $@ $^
	
ata.o: ata.c
	$(CC) $(CFLAGS) -o $@ $^
	
control.o: control.c
	$(CC) $(CFLAGS) -o $@ $^

detect.o: detect.c
	$(CC) $(CFLAGS) -o $@ $^

registers.o: registers.c
	$(CC) $(CFLAGS) -o $@ $^

transfer.o: transfer.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: ata-a-clean ata-o-clean control-o-clean detect-o-clean registers-o-clean transfer-o-clean

ata-a-clean:
	$(RM) ata.a

ata-o-clean:
	$(RM) ata.o

control-o-clean:
	$(RM) control.o

detect-o-clean:
	$(RM) detect.o
	
registers-o-clean:
	$(RM) registers.o
	
transfer-o-clean:
	$(RM) transfer.o