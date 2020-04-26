; kate: syntax Intel x86 (NASM);

MEMORY_MAP_MAGIC_WORD	equ	0x534D4150
MEMORY_MAP_BIOS_FUNC	equ	0xE820
MEMORY_MAP_DI_INC	equ	24

STACK_POINTER_INIT	equ	0x9C00

BITS 16

ORG 0x7C00

; initialize A20
mov ax, 0x2401
int 0x15
  
; set VGA mode 3
;mov ax, 0x3
;int 0x10

; save boot disk
mov [bootDisk], dl

; set up segment selectors and stack
xor ax, ax
mov ds, ax
mov ss, ax
mov sp, STACK_POINTER_INIT

cli

push ds

lgdt [gdtPtr]

mov  eax, cr0
inc eax
mov  cr0, eax

jmp $+3
nop
nop

mov  bx, 0x08
mov  ds, bx

mov eax, cr0
dec eax
mov cr0, eax

pop ds
sti

; move bootloader sectors into memory
mov ah, 0x2		; BIOS function - read sectors
mov al, [numSectors] 	; # of sectors
mov ch, 0 		; cylinder
mov dh, 0 		; head
mov cl, 2 		; sector
mov dl, [bootDisk]	; disk
mov bx, [loadTarget]	;location to load into
int 0x13

mov dl, [bootDisk]
mov cl, [numSectors]

jmp 0x7E00

hlt

gdtPtr:
	dw gdt_end - gdt - 1
	dd gdt
	gdt		dd 0,0
	flatdesc	db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0

gdt_end:

bootDisk	db	0x0
numSectors	db	0x3
loadTarget	dw	0x7E00
  
times 510 - ($-$$) db 0

dw 0xAA55