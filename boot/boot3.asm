; kate: syntax Intel x86 (NASM);

BITS 64
mov rax, [kernelAddress]

;jmp [rax]

kernelAddress	dq	0xFFFF800000000000

times 512 - ($-$$) db 0