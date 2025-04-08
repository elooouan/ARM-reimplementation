.global _start

.section .text
_start:
    mov r0, #0
    mov r7, #1
    svc #0

.section .data
msg: .asciz "Hello, World!\n"
    
.section .bss
buffer: .skip 64
    db 0xFF    ; Cette ligne est incorrecte car elle ne doit pas être dans cette section
