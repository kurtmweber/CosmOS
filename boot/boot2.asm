; kate: syntax Intel x86 (NASM);

BITS 32

ORG 0x7E00

mov esi, pModeMsg
mov ebx, 0xB80A0

pModeMsgLoop:
  lodsb
  or al, al
  jz done
  or eax, 0x0F00
  mov word [ebx], ax
  add ebx, 2
  jmp pModeMsgLoop

done:
  cli
  hlt
  
pModeMsg:
  db "In 32-bit protected-mode...", 0

times 510 - ($-$$) db 0
dw 0xAA55