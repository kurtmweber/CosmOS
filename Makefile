MAKE=make
LD=ld
RM=rm

all: boot-subsystem kernel-subsystem

boot-subsystem:
	cd boot && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE)
	
clean: boot-clean kernel-clean
	$(RM) -f JustOS

kernel-clean:
	cd kernel && $(MAKE) clean

boot-clean:
	cd boot && $(MAKE) clean