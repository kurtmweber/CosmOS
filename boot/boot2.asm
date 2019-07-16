; kate: syntax Intel x86 (NASM);

[map all boot2.map]

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
	
kernelLoad:
	xor ecx, ecx
	mov cl, byte [numKernelSectors]
	xor ebx, ebx
	xor edx, edx
	
	.loadLoop:
		push ecx
		
		mov ah, 0x2
		mov al, 1
		mov ch, 0
		mov dh, 0
		mov cl, byte [kernelBaseSector]
		add cl, bl
		push ebx
		mov dl, byte [bootDisk]
		mov bx, 0x500
		int 0x13
		pop ebx

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
			
		pop ebx
		inc ebx
		pop ecx
		
		loop .loadLoop
	
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

; switch back to pmode
cli

mov eax, cr0
inc eax
mov cr0, eax

;[BITS 32]

; load page table address into cr3
mov edi, [pml4Base]
mov cr3, edi

; clear the PML4 area
xor eax, eax
mov ecx, 4096
rep stosb

mov edi, cr3

; PML4 entry
mov ebx, [pml4Entry]
mov [edi], ebx

; clear the pdpt area
mov edi, [pdptBase]
mov ecx, 4096
rep stosb
mov edi, [pdptBase]

; PDPT entry
mov ebx, [pdptEntry]
mov [edi], ebx

; clear the pdt area
mov edi, [pdtBase]
mov ecx, 4096
rep stosb
mov edi, [pdtBase]

; PDT entry
mov ebx, [pdtEntry]
mov [edi], ebx

; clear the pt area
mov edi, [ptBase]
mov ecx, 4096
rep stosb
mov edi, [ptBase]

; identity-map first megabyte
mov ecx, 256
mov ebx, 0x00000003

idMap1:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop idMap1

; enable PAE
mov eax, cr4
or eax, 1 << 5	; pae is bit 5
mov cr4, eax

; set EFER.LME
mov ecx, DWORD 0xC0000080
rdmsr
or eax, 1 << 8
wrmsr

; turn on paging
mov eax, cr0
or eax, 1 << 31
mov cr0, eax

jmp clr64

nop

nop

clr64:

; go to pure 64-bit mode
lgdt [gdt64.ptr]

jmp enter64


enter64:

; now that we're in pure 64-bit mode, we'll set up the page tables for our
; higher-half kernel and our kernel stack (immediately below the permanent
; page table area at -1 TB, though the permanent page table area will be done
; in the kernel)

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
mov ebx, 0x00100003

loadKernel:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop loadKernel

mov edi, 0xB8000

mov eax, "H"
mov [edi], eax
inc edi
mov eax, 0x0F
mov [edi], eax

hlt

bootDisk 		db	0
numKernelSectors	db	1
kernelBaseSector	db	0
loadTarget		dd	0x100000

pml4Base	dd	0x00010000
pml4Entry	dd	0x00011003
pdptBase	dd	0x00011000
pdptEntry	dd	0x00012003
pdtBase		dd	0x00012000
pdtEntry	dd	0x00013003
ptBase		dd	0x00013000

pml4KernelBase	dq	0x00010800
pml4KernelEntry	dq	0x00014003
pdptKernelBase	dq	0x00014000
pdptKernelEntry	dq	0x00015003
pdtKernelBase	dq	0x00015000
pdtKernelEntry	dq	0x00016003
ptKernelBase	dq	0x00016000

; 64-bit GDT
gdt64:
	.null:	equ	$ - gdt64
		dw	0xFFFF
		dw	0
		db	0
		db	0
		db	1
		db	0
	.code:	equ	$ - gdt64
		dw	0
		dw	0
		db	0
		db	10011010b
		db	10101111b
		db	0
	.data:	equ	$ - gdt64
		dw	0
		dw	0
		db	0
		db	10010010b
		db	0
		db	0
	.ptr:	equ	$ - gdt64
		dw	$ - gdt64 - 1
		dq	gdt64
		

times 1024 - ($-$$) db 0