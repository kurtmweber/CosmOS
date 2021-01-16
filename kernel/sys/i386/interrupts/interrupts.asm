BITS 64

%macro pushaq 0
push rax
push rbx
push rcx
push rdx
push rbp
push rdi
push rsi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
%endmacro

%macro popaq 0
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rsi
pop rdi
pop rbp
pop rdx
pop rcx
pop rbx
pop rax
%endmacro

global irq0;
global irq1;
global irq2;
global irq3;
global irq4;
global irq5;
global irq6;
global irq7;
global irq8;
global irq9;
global irq10;
global irq11;
global irq12;
global irq13;
global irq14;
global irq15;

global isrDE;
global isrPFE;

extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler
 
extern isrDE_handler
extern isrPFE_handler

irq0:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq0_handler
    popaq
    iretq

irq1:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq1_handler
    popaq
    iretq
 
 irq2:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq2_handler
    popaq
    iretq

 irq3:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq3_handler
    popaq
    iretq

 irq4:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq4_handler
    popaq
    iretq

 irq5:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq5_handler
    popaq
    iretq

 irq6:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq6_handler
    popaq
    iretq

 irq7:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq7_handler
    popaq
    iretq

 irq8:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq8_handler
    popaq
    iretq

irq9:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq9_handler
    popaq
    iretq

 irq10:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq10_handler
    popaq
    iretq

 irq11:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq11_handler
    popaq
    iretq

 irq12:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq12_handler
    popaq
    iretq

 irq13:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq13_handler
    popaq
    iretq

 irq14
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq14_handler
    popaq
    iretq

 irq15:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call irq15_handler
    popaq
    iretq

 isrDE:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call isrDE_handler
    popaq
    iretq


isrPFE:
    cli
    pushaq
    xor ax, ax
    mov es, ax
    mov ds, ax
    cld
    xor rax, rax
    mov rdi, rsp
    call isrPFE_handler
    popaq
    iretq
    