include ../../../mk/build.mk

EXTRA_CFLAGS=-I../..
ARFLAGS=-crs

all: ata.a

ata.a: ata.o control.o detect.o registers.o transfer.o
	$(AR) $(ARFLAGS) $@ $^
	
ata.o: ata.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
control.o: control.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

detect.o: detect.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

registers.o: registers.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

transfer.o: transfer.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a
