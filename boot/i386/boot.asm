; kate: syntax Intel x86 (NASM);

STACK_POINTER_INIT	equ	0x9C00

BITS 16
ORG 0x7C00

cli

; set video mode
mov ah, 0x0
mov al, 0x3
int 0x10

; save boot drive
mov [bootDisk], dl

; enable a20
mov ax, 0x2401
int 0x15

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

jmp [loadTarget]


hlt

; GDT
gdtPtr:
	dw gdt_end - gdt - 1
	dd gdt
	gdt		dd 0,0
	dataDesc	db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
	codeDesc	db 0xff, 0xff, 0, 0, 0, 10011010b, 11001111b, 0
gdt_end:

; data reservations
bootDisk	db	0x0
numSectors	db	0x3
loadTarget	dw	0x7E00

times 512 - 2 - ($ - $$) db 0 ; zero-pad the 512-byte sector to the last 2 bytes
dw 0xaa55 ; Magic "boot signature"
