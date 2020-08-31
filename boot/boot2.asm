; kate: syntax Intel x86 (NASM);

MEMORY_MAP_MAGIC_WORD	equ	0x534D4150
MEMORY_MAP_BIOS_FUNC	equ	0xE820
MEMORY_MAP_DI_INC	equ	24

BITS 16

ORG 0x7E00

; we inherit the bootdisk in dl from the first-stage loader
; copy it to a safe place that we won't overwrite--even though
; it's not necessary as I write this, dl might be overwritten
; as I expand this, so we'll save it proactively to avoid problems
; in the future
mov byte [bootDisk], dl

; the number of sectors of the second-stage bootloader is passed to us in cl
; we add two to it to get the sector number at which the kernel starts
; formula = sectors of second-stage bootloader + 1 for first-stage bootloader + 1 for the first sector of the kernel
; (sector #s start at 1 rather than 0, for some weird reason)
add cl, 2
mov byte [kernelBaseSector], cl
mov byte [curLoadSector], cl

kernelLoad:
	xor ecx, ecx
	mov cx, word [numKernelSectors]
	xor ebx, ebx
	xor edx, edx
	
	loadLoop:
		push ecx
		xor ecx, ecx
		
		mov ah, 0x2
		mov al, 1
		
		; cylinder is 10 bits, high 8 bits are in ch, low 2 bits are the high 2 bits of cl
		mov cx, word [curLoadCylinder]
		shl cx, 6
		; and then the low 6 bits of cl are the sector, so we or them so we don't clear the upper 2 bits as we might if we moved an entire byte
		or cl, byte [curLoadSector]
		;add cl, bl
		push ebx
		mov dh, [curLoadHead]
		mov dl, byte [bootDisk]
		mov bx, 0x500
		int 0x13
		pop ebx
		
		; now we want the sector number itself, so we increment the memory location and move it into cl for comparison purposes
		inc byte [curLoadSector]
		mov cl, [curLoadSector]
		cmp cl, [secPerTrack]
		jbe contCopy	; jbe instead of jb because sector numbering starts at 1, so sector = 63 is a valid condition
		
		; otherwise...we reset the sector # to 1, and increment the head #
		mov byte [curLoadSector], 1
		inc byte [curLoadHead]
		
		; and now we need to see if we're at the max sector #, and if so reset it and increment the cylinder #
		mov cl, byte [curLoadSector]
		cmp cl, [numHeads]
		jb contCopy	; jb here since heads start at 0, so head maxes out at 15
		
		inc word [curLoadCylinder]
		
	contCopy:
		
		jmp doSectorCopy
		
	retSectorCopy:
			
		pop ebx
		inc ebx
		pop ecx
		
		loop loadLoop
		
		jmp getMemMap
		
doSectorCopy:
		; after loading each sector from disk, copy it byte-by-byte into the proper location
		mov ecx, 512
		
		mov edx, dword [loadTarget]
		
		mov eax, ebx	; copy ebx (zero-based index of the current sector being loaded) to eax
		push ebx	; save ebx because we need all four general-purpose registers for this operation and we have to hold on to the
				; current ebx value
		shl eax, 9	; multiply eax by 512 - this is the index for the first byte of this sector
		add edx, eax	; edx is our target address, so we add (512 * current sector idx) to loadTarget that we copied here earlier
		mov eax, 0x500
		
		; eax - address from which to copy
		; bl - transfer register
		; ecx - counter
		; edx - destination
		
		.sectorCopy:
			mov bl, [eax]
			mov [edx], bl
			inc eax
			inc edx
			loop .sectorCopy
			
		jmp retSectorCopy

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

failMemMap:	; TODO: add some sort of error message
hlt

doneMemMap:

; write 24 clear bytes at end of map
; this will be our flag
add di, MEMORY_MAP_DI_INC
mov ax, 0x00
mov ecx, MEMORY_MAP_DI_INC
rep stosb

; switch back to pmode
cli

mov eax, cr0
inc eax
mov cr0, eax

jmp 0x10:doPaging

BITS 32

doPaging:
; disable pmode paging
mov eax, cr0
and eax, 01111111111111111111111111111111b
mov cr0, eax

mov eax, 0x08
mov ds, eax
mov es, eax
mov fs, eax
mov gs, eax
mov ss, eax

; clear page table area
; this bare-bones page table consists of four levels of table, each of 4096 bytes
; so if we write 4096 dwords, we can clear it all in one go

mov edi, [pml4Base]	
mov cr3, edi
xor eax, eax
mov ecx, 4096
rep stosd
mov edi, cr3

mov eax, [pml4Entry]
mov DWORD [edi], eax
add edi, 0x1000
mov eax, [pdptEntry]
mov DWORD [edi], eax
add edi, 0x1000
mov eax, [pdtEntry]
mov DWORD [edi], eax
add edi, 0x1000

; identity-map first megabyte
mov ebx, 0x00000003
mov ecx, 256
 
.SetEntry:
    mov DWORD [edi], ebx         ; Set the uint32_t at the destination index to the B-register.
    add ebx, 0x1000              ; Add 0x1000 to the B-register.
    add edi, 8                   ; Add eight to the destination index.
    loop .SetEntry               ; Set the next entry.
    
mov eax, cr4                 ; Set the A-register to control register 4.
or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
mov cr4, eax                 ; Set control register 4 to the A-register.

mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
rdmsr                        ; Read from the model-specific register.
or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
wrmsr                        ; Write to the model-specific register.

mov eax, cr0                 ; Set the A-register to control register 0.
or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
mov cr0, eax                 ; Set control register 0 to the A-register.

lgdt [GDT64.Pointer]         ; Load the 64-bit global descriptor table.
jmp GDT64.Code:Realm64       ; Set the code segment and enter 64-bit long mode.

; Use 64-bit.
[BITS 64]
 
Realm64:
	mov ax, GDT64.Data
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; map kernel area
	; kernel PML4 entry
	mov edi, [pml4KernelBase]
	mov ebx, [pml4KernelEntry]
	mov [edi], ebx

	xor eax, eax

	; clear PDPT area
	mov edi, [pdptKernelBase]
	mov ecx, 4096
	rep stosb

	; kernel PDPT entry
	mov edi, [pdptKernelBase]
	mov ebx, [pdptKernelEntry]
	mov [edi], ebx

	; clear PDT area
	mov edi, [pdtKernelBase]
	mov ecx, 4096
	rep stosb

	; kernel PDT entry
	mov edi, [pdtKernelBase]
	mov ebx, [pdtKernelEntry]
	mov [edi], ebx

	; clear PT area
	mov edi, [ptKernelBase]
	mov ecx, 4096
	rep stosb

	; load kernel PT area
	mov edi, [ptKernelBase]
	mov ecx, 512
	mov ebx, 0x00900003

	; map the two megabytes starting at 0x100000 to the bottom of the available
	; higher-half area
	
	mapKernel:
		mov DWORD [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop mapKernel
		
	; and now we map the kernel stack area
	

    jmp [boot3]

    hlt


; data definitions
bootDisk 		db	0
numKernelSectors	dw	79
kernelBaseSector	db	1
curLoadSector		db	0
curLoadHead		db	0
curLoadCylinder		dw	0
numHeads		db	16
secPerTrack		db	63
loadTarget		dd	0x900000
boot3			dq	0x8200

pml4Base	dd	0x00010000
pml4Entry	dd	0x00011003
pdptBase	dd	0x00011000
pdptEntry	dd	0x00012003
pdtBase		dd	0x00012000
pdtEntry	dd	0x00013003
ptBase		dd	0x00013000

pml4KernelBase	dd	0x00010800
pml4KernelEntry	dd	0x00014003
pdptKernelBase	dd	0x00014000
pdptKernelEntry	dd	0x00015003
pdtKernelBase	dd	0x00015000
pdtKernelEntry	dd	0x00016003
ptKernelBase	dd	0x00016000

GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0xFFFF                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.

times 1024 - ($-$$) db 0