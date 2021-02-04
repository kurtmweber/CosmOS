; kate: syntax Intel x86 (NASM);

ORG 0x8200

BITS 64

cli                           ; Clear the interrupt flag.

; set up pages for kernel stack
xor rax, rax
add rax, [pdptKstackBase]
add rax, 3
mov rbx, [pml4Base]
add rbx, 511 * 8
mov [rbx], rax

; place pdpt entry
xor rax, rax
add rax, [pdtKstackBase]
add rax, 3
mov rbx, [pdptKstackBase]
add rbx, 511 * 8
mov [rbx], rax

; place pdt entries
; four of them, at indices 508, 509, 510, and 511
xor rax, rax
add rax, [ptKstackBase1]
add rax, 3
mov rbx, [pdtKstackBase]
add rbx, 508 * 8
mov [rbx], rax
add rax, 0x1000
add rbx, 8
mov [rbx], rax
add rax, 0x1000
add rbx, 8
mov [rbx], rax
add rax, 0x1000
add rbx, 8
mov [rbx], rax

; and finally, fill out the page table entries, eight megabytes
; starting at 0x100000

mov edi, [ptKstackBase1]
mov ecx, 512
mov ebx, [kStackLoc1]
add ebx, 3
.fillK1:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .fillK1

mov edi, [ptKstackBase2]
mov ecx, 512
mov ebx, [kStackLoc2]
add ebx, 3
.fillK2:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .fillK2
	
mov edi, [ptKstackBase3]
mov ecx, 512
mov ebx, [kStackLoc3]
add ebx, 3
.fillK3:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .fillK3
	
mov edi, [ptKstackBase4]
mov ecx, 512
mov ebx, [kStackLoc4]
add ebx, 3
.fillK4:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .fillK4
	
; set the new stack pointer
mov rsp, 0xfffffffffffffff8
    
mov rax, [kernelAddress]

jmp rax

kernelAddress	dq	0xFFFF800000000000
    
pml4Base	dq	0x00010000

pdptKstackBase	dq	0x00020000
pdtKstackBase	dq	0x00021000
ptKstackBase1	dq	0x00022000
ptKstackBase2	dq	0x00023000
ptKstackBase3	dq	0x00024000
ptKstackBase4	dq	0x00025000

kStackLoc1	dq	0x001000000
kStackLoc2	dq	0x001200000
kStackLoc3	dq	0x001400000
kStackLoc4	dq	0x001600000

times 512 - ($-$$) db 0