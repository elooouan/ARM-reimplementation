.global _start

.section .text
_start:
    mov r0, #42
    mov r7, #1
    svc #0

.section .data
var1: .word 0x12345678

.section .bss
var2: .skip 4
