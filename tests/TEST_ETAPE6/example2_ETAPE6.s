.global _start

.section .text
_start:
    ldr r2, =var2  @ Chargement de l'adresse de var2
    mov r7, #1
    svc #0

.section .bss
var2: .skip 4
