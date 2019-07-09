; kate: syntax Intel x86 (NASM);

PMODE_MSG_BASE	equ	0xB8000
KERNEL_MSG_BASE	equ	0xB80A0
DONE_MSG_BASE	equ	0xB80C2

BITS 32

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

mov esi, pModeMsg
mov ebx, PMODE_MSG_BASE

pModeMsgLoop:
	lodsb
	or al, al
	jz kernelLoadMsgSetup
	or eax, 0x0F00
	mov word [ebx], ax
	add ebx, 2
	jmp pModeMsgLoop
  
kernelLoadMsgSetup:
	mov esi, kernelLoadMsg
	mov ebx, KERNEL_MSG_BASE

kernelLoadMsgLoop:
	lodsb
	or al, al
	jz kernelLoad
	or eax, 0x0F00
	mov word [ebx], ax
	add ebx, 2
	jmp kernelLoadMsgLoop
	
	
kernelLoad:
	xor ecx, ecx
	mov cl, byte [numKernelSectors]
	xor ebx, ebx
	
	.loadLoop:
		push ecx

		mov eax, cr0
		dec eax
		mov cr0, eax
		
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
		
		mov eax, cr0
		inc eax
		mov cr0, eax

BITS 32
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
		
mov esi, kernelDoneMsg
mov ebx, DONE_MSG_BASE

kernelDoneMsgLoop:
	lodsb
	or al, al
	jz pagingMsg
	or eax, 0x0F00
	mov word [ebx], ax
	add ebx, 2
	jmp kernelDoneMsgLoop

pagingMsg:
	cli
	hlt

pModeMsg	db	"In 32-bit protected-mode...", 0
  
kernelLoadMsg	db	"Loading kernel...", 0

kernelDoneMsg	db	"done", 0

bootDisk 		db	0
numKernelSectors	db	1
kernelBaseSector	db	0
loadTarget		dd	0x100000

times 510 - ($-$$) db 0
dw 0xAA55