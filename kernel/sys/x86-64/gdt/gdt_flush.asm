
BITS 64
DEFAULT REL

global gdt_flush
extern gdt_pointer

gdt_flush:
    lgdt [gdt_pointer]

    mov rax, 0x10  ; data
    mov ds, rax
    mov es, rax
    mov fs, rax
    mov gs, rax
    mov ss, rax

    mov rax, .flush
    push 0x08     ; code
    push rax
    retf    
.flush:
    ret
