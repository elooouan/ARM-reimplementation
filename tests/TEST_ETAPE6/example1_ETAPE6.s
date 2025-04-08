.global _start

.section .text
_start:
    ldr r1, =var1  @ Chargement de l'adresse de var1
    mov r7, #1
    svc #0

.section .data
var1: .word 0xABCD1234
