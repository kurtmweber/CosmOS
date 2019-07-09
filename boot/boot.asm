; kate: syntax Intel x86 (NASM);

MEMORY_MAP_MAGIC_WORD	equ	0x534D4150
MEMORY_MAP_BIOS_FUNC	equ	0xE820
MEMORY_MAP_DI_INC	equ	24

BITS 16

ORG 0x7C00

; initialize A20
mov ax, 0x2401
int 0x15
  
; set VGA mode 3
mov ax, 0x3
int 0x10

; save boot disk
mov [bootDisk], dl

; move bootloader sectors into memory
mov ah, 0x2		; BIOS function - read sectors
mov al, [numSectors] 	; # of sectors
mov ch, 0 		; cylinder
mov dh, 0 		; head
mov cl, 2 		; sector
mov dl, [bootDisk]	; disk
mov bx, [loadTarget]	;location to load into
int 0x13

; get BIOS memory map

getMemMap:

xor ebx, ebx
mov es, bx
mov di, 0x0500		; beginning of first free-memory area
mov edx, MEMORY_MAP_MAGIC_WORD
xor eax, eax
mov eax, MEMORY_MAP_BIOS_FUNC
mov ecx, MEMORY_MAP_DI_INC
int 0x15

contMemMap:

cmp eax, MEMORY_MAP_MAGIC_WORD
jne failMemMap
mov eax, MEMORY_MAP_BIOS_FUNC
add di, MEMORY_MAP_DI_INC
int 0x15
cmp ebx, 0
jz doneMemMap
jc doneMemMap
jmp contMemMap

failMemMap:	; add some sort of error message
hlt

doneMemMap:


; set up a basic protected mode environment
; we're just here to get to long mode, so we can keep it simple
cli

lgdt [gdtPtr]
mov eax, cr0
inc eax
mov cr0, eax

mov ax, dataSeg
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

sti

mov dl, [bootDisk]
mov cl, [numSectors]

; go to the second-stage bootloader
jmp codeSeg:0x7E00

gdtStart:
  dq 0x0;
gdtCodeSeg:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0
gdtDataSeg:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0
gdtEnd:

gdtPtr:
  dw gdtEnd - gdtStart
  dd gdtStart
  
codeSeg equ gdtCodeSeg - gdtStart
dataSeg equ gdtDataSeg - gdtStart

bootDisk	db	0x0
numSectors	db	0x1
loadTarget	dw	0x7E00
  
times 510 - ($-$$) db 0

dw 0xAA55